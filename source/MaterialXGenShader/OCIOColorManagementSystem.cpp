//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/OCIOColorManagementSystem.h>

#include <MaterialXGenShader/GenContext.h>
#include <MaterialXGenShader/ShaderGenerator.h>

#include <iostream>

#ifdef OpenColorIO_SKIP_IMPORTS
#error "OCIO has been build as a non-static library"
#endif
#pragma warning(disable: 4275)
#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

namespace MaterialX
{

const string OCIOColorManagementSystem::CMS_NAME = "ocio_cms";
const int OCIO_UNSUPPORT_LANGUAGE = -1;

using OCIOLanguageMap = std::unordered_map<std::string, OCIO::GpuLanguage>;

// OCIO related information
class OCIOInformation
{
public:
    // OCIO config file to use
    OCIO::ConstConfigRcPtr config = nullptr;
    // Mapping from supported MaterialX targets to OCIO shading languages
    OCIOLanguageMap languageMap;
    // Language used to code generate with (is a OCIO::GpuLanguage)
    int language = OCIO_UNSUPPORT_LANGUAGE;
    // MaterialX target
    std::string target;
};

OCIOColorManagementSystemPtr OCIOColorManagementSystem::create(const string& target)
{
    return OCIOColorManagementSystemPtr(new OCIOColorManagementSystem(target));
}

OCIOColorManagementSystem::OCIOColorManagementSystem(const string& target) :
    _target(target)
{
    _ocioInfo = new OCIOInformation();
    _ocioInfo->config = nullptr;
    _ocioInfo->languageMap["genglsl"] = OCIO::GPU_LANGUAGE_GLSL_4_0;

    _targetSupported = false;
}

void OCIOColorManagementSystem::loadLibrary(DocumentPtr document) 
{
    _ocioInfo->language = OCIO_UNSUPPORT_LANGUAGE;

    _document = document;
    if (!_document)
    {
        throw ExceptionShaderGenError("No library loaded for color management system");
    }

    TargetDefPtr targetDef = _document->getTargetDef(_target);
    const StringVec targets = targetDef->getMatchingTargets();

    for (const string& target : targets)
    {
        if (_ocioInfo->languageMap.count(target))
        {
            _ocioInfo->language = static_cast<int>(OCIO::GPU_LANGUAGE_GLSL_4_0);
            _ocioInfo->target = target;
            break;                
        }
    }
}

bool OCIOColorManagementSystem::setConfigFile(const FilePath& configFile)
{
    if (_configFile != configFile && configFile.size())
    {
        _configFile = configFile;
        _colorSpaceNames.clear();

        _ocioInfo->config = OCIO::Config::CreateFromFile(_configFile.asString().c_str());
        if (!_ocioInfo->config)
        {
            OCIO::GetCurrentConfig();
        }
        if (_ocioInfo->config)
        {
            OCIO::ColorSpaceSetRcPtr colorSpaces = _ocioInfo->config->getColorSpaces(nullptr);
            int colorSpaceCount = colorSpaces->getNumColorSpaces();
            for (int i = 0; i < colorSpaceCount; i++)
            {
                _colorSpaceNames.insert(colorSpaces->getColorSpaceNameByIndex(i));
            }
            return true;
        }
    }
    return false;
}

/// Returns whether this color management system supports a provided transform
bool OCIOColorManagementSystem::supportsTransform(const ColorSpaceTransform& transform) const
{
    if (_ocioInfo->language == OCIO_UNSUPPORT_LANGUAGE)
    {
        return false;
    }

    OCIO::ConstProcessorRcPtr processor = _ocioInfo->config->getProcessor(transform.sourceSpace.c_str(), transform.targetSpace.c_str());
    return (nullptr != processor);
}

/// Create a node to use to perform the given color space transformation.
ShaderNodePtr OCIOColorManagementSystem::createNode(const ShaderGraph* parent, const ColorSpaceTransform& transform, const string& name, GenContext& context) const
{
    if (transform.type != Type::COLOR3 && transform.type != Type::COLOR4)
    {
        throw ExceptionShaderGenError("Invalid type specified to color transform: '" + transform.type->getName() + "'");
    }

    ImplementationPtr impl = getImplementation(transform);

    if (!impl)
    {
        throw ExceptionShaderGenError("No implementation found for transform: ('" + transform.sourceSpace + "', '" + transform.targetSpace + "').");
    }

    // Check if it's created and cached already, otherwise create and cache it.
    ShaderNodeImplPtr nodeImpl = context.findNodeImplementation(impl->getName());
    if (!nodeImpl)
    {
        // Note: Use custom OCIO "source code node".
        nodeImpl = OCIOSourceCodeNode::create();
        nodeImpl->initialize(*impl, context);
        context.addNodeImplementation(impl->getName(), nodeImpl);
    }

    // Create the node.
    ShaderNodePtr shaderNode = ShaderNode::create(parent, name, nodeImpl, ShaderNode::Classification::TEXTURE);

    // Create ports on the node.
    ShaderInput* input = shaderNode->addInput("in", transform.type);
    if (transform.type == Type::COLOR3)
    {
        input->setValue(Value::createValue(Color3(0.0f, 0.0f, 0.0f)));
    }
    else if (transform.type == Type::COLOR4)
    {
        input->setValue(Value::createValue(Color4(0.0f, 0.0f, 0.0f, 1.0)));
    }
    else
    {
        throw ExceptionShaderGenError("Invalid type specified to color transform: '" + transform.type->getName() + "'");
    }
    shaderNode->addOutput("out", transform.type);

    return shaderNode;
}

ImplementationPtr OCIOColorManagementSystem::getImplementation(const ColorSpaceTransform& transform) const
{
    if (!_document)
    {
        return nullptr;
    }
    if (_ocioInfo->language == OCIO_UNSUPPORT_LANGUAGE)
    {
        return nullptr;
    }
    OCIO::ConstConfigRcPtr config = _ocioInfo->config;
    if (!config)
    {
        return nullptr;
    }

    // Set up process for a color space transform
    try
    {
        OCIO::ConstProcessorRcPtr processor = config->getProcessor(transform.sourceSpace.c_str(), transform.targetSpace.c_str());
        if (!processor)
        {
            return nullptr;
        }

        // Get a GPU processor
        OCIO::ConstGPUProcessorRcPtr gpu = processor->getDefaultGPUProcessor();
        if (!gpu)
        {
            return nullptr;
        }

        // Create a shader descriptor
        OCIO::GpuShaderDescRcPtr shaderDesc = OCIO::GpuShaderDesc::CreateShaderDesc();
        if (!shaderDesc)
        {
            return nullptr;
        }

        shaderDesc->setLanguage(static_cast<OCIO::GpuLanguage>(_ocioInfo->language));
        std::string transformFunctionName = "IM_" + transform.sourceSpace + "_to_" + transform.targetSpace + "_" + transform.type->getName() + "_ocio";
        shaderDesc->setFunctionName(transformFunctionName.c_str());

        // TODO : OR Instead of GpuShaderDesc, the may need to derive from GpuShaderCreator

        // Retrieve information
        gpu->extractGpuShaderInfo(shaderDesc);

        std::string fullFunction = shaderDesc->getShaderText();
        std::string functionName = shaderDesc->getFunctionName();
        std::string outputName = shaderDesc->getPixelName();

        std::cout << "Created function: " + functionName << std::endl;
        std::cout << "Function body: " + fullFunction << std::endl;
        std::cout << "Output name: " + outputName << std::endl;

        // Produces this for acescg to lin_rec709:
        /*
        // Declaration of the OCIO shader function

        vec4 IM_acescg_to_lin_rec709_color3_ocio(in vec4 inPixel)
        {
            vec4 outColor = inPixel;

            // Add a Matrix processing

            outColor = mat4(1.7049986809609248, -0.13025866763271018, -0.024007514741958086, 0., -0.62174765641381313,
            1.1408020008115505, -0.12898266957287285, 0., -0.083251260888346112, -0.010544731974266384, 1.1529906045288774, 0., 0., 0., 0., 1.)
            * outColor;

            return outColor;
        }
        */

        // TODO : Create an implementation based on source code
        // Note: A separate utility could take OCIO shader code and generate a set of MTLX implementations.
        //       This could be useful to update the DefaultColorSystems implementations to maintain 
        //       consistency with OCIO.
        ImplementationPtr impl = _document->addImplementation(transformFunctionName);
        if (impl)
        {
            // Q: How to parse inputs ?
            impl->addInput("inPixel", transform.type->getName());
            impl->addOutput(outputName, transform.type->getName());
            impl->setAttribute("sourcecode", fullFunction);
            impl->setAttribute("function", functionName);
            impl->setAttribute("target", _ocioInfo->target);
        }
        return impl;
    }
    catch (const OCIO::Exception& e)
    {
        throw ExceptionShaderGenError("Code generation failed for transform: '" + 
            transform.sourceSpace + "' to '" + transform.targetSpace + "'. Type: '" + transform.type->getName()  + "' : " + e.what());
    }

    return nullptr;
}

//
// OCIO source code node method
//

ShaderNodeImplPtr OCIOSourceCodeNode::create()
{
    return std::make_shared<OCIOSourceCodeNode>();
}

void OCIOSourceCodeNode::emitFunctionDefinition(const ShaderNode&, GenContext& context, ShaderStage& stage) const
{
    // Write source code as is
    BEGIN_SHADER_STAGE(stage, Stage::PIXEL)
    if (!_functionSource.empty())
    {
        const ShaderGenerator& shadergen = context.getShaderGenerator();
        shadergen.emitBlock(_functionSource, context, stage);
        shadergen.emitLineBreak(stage);
    }
    END_SHADER_STAGE(stage, Stage::PIXEL)
}

void OCIOSourceCodeNode::emitFunctionCall(const ShaderNode& node, GenContext& context, ShaderStage& stage) const
{
    // TODO: Put in correct code here. Just a copy of HwSourceCode
    BEGIN_SHADER_STAGE(stage, Stage::PIXEL)
    if (_inlined)
    {
        SourceCodeNode::emitFunctionCall(node, context, stage);
    }
    else
    {
        const ShaderGenerator& shadergen = context.getShaderGenerator();

        // Declare the output variable
        shadergen.emitLineBegin(stage);
        shadergen.emitOutput(node.getOutput(0), true, false, context, stage);
        shadergen.emitString(" = ", stage);

        // Emit function name. TODO add vec3/vec4 conversion as needed
        shadergen.emitString(_functionName + "(", stage);

        // Emit all inputs.
        string delim = "";
        for (ShaderInput* input : node.getInputs())
        {
            shadergen.emitString(delim, stage);
            shadergen.emitInput(input, context, stage);
            delim = ", ";
        }

        // End function call
        shadergen.emitString(")", stage);
        shadergen.emitLineEnd(stage);
    }
    END_SHADER_STAGE(stage, Stage::PIXEL)
}



} // namespace MaterialX
