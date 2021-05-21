//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/OCIOColorManagementSystem.h>

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

// OCIO related information
class OCIOInformation
{
public:
    OCIO::ConstConfigRcPtr config;
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
bool OCIOColorManagementSystem::supportsTransform(const ColorSpaceTransform& /*transform*/) const
{
    return false;
}

/// Create a node to use to perform the given color space transformation.
ShaderNodePtr OCIOColorManagementSystem::createNode(const ShaderGraph* /*parent*/, const ColorSpaceTransform& transform, const string& /*name*/, GenContext& /*context*/) const
{
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

        shaderDesc->setLanguage(OCIO::GPU_LANGUAGE_GLSL_4_0);
        std::string transformFunctionName = "IM_" + transform.sourceSpace + "_to_" + transform.targetSpace + "_" + transform.type->getName() + "_ocio";
        shaderDesc->setFunctionName(transformFunctionName.c_str());

        // TODO : OR Instead of GpuShaderDesc, the may need to derive from GpuShaderCreator

        // Retrieve information
        gpu->extractGpuShaderInfo(shaderDesc);

        std::string fullFunction = shaderDesc->getShaderText();
        std::string functionName = shaderDesc->getFunctionName();

        std::cout << "Created function: " + functionName << std::endl;
        std::cout << "Function body: " + fullFunction << std::endl;

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
    }
    catch (const OCIO::Exception& )
    {
    }

    return nullptr;
}

ImplementationPtr OCIOColorManagementSystem::getImplementation(const ColorSpaceTransform& transform) const
{
    if (!_document)
    {
        throw ExceptionShaderGenError("No library loaded for color management system");
    }

    TargetDefPtr targetDef = _document->getTargetDef(_target);
    const StringVec targets = targetDef->getMatchingTargets();
    
    for (const string& target : targets)
    {
        const string implName = "IM_" + transform.sourceSpace + "_to_" + transform.targetSpace + "_" + transform.type->getName() + "_" + target;
        ImplementationPtr impl = nullptr; // _document->getImplementation(implName);
        if (impl)
        {
            return impl;
        }        
    }

    return nullptr;
}

} // namespace MaterialX
