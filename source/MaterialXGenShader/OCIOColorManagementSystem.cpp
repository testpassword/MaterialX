//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/OCIOColorManagementSystem.h>

#include <MaterialXGenShader/ShaderGenerator.h>

#ifdef MATERIALX_BUILD_OCIO
#ifdef OpenColorIO_SKIP_IMPORTS
#error "OCIO has been build as a non-static library"
#endif
#pragma warning(disable: 4275)
#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;
#endif

namespace MaterialX
{

const string OCIOColorManagementSystem::CMS_NAME = "ocio_cms";

class OCIOInformation
{
public:
    OCIO::ConstConfigRcPtr config;
};

//
// OCIOColorManagementSystem methods
//

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

OCIOColorManagementSystemPtr OCIOColorManagementSystem::create(const string& target)
{
    return OCIOColorManagementSystemPtr(new OCIOColorManagementSystem(target));
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

OCIOColorManagementSystem::OCIOColorManagementSystem(const string& target) :
    _target(target)
{
#ifdef MATERIALX_BUILD_OCIO
    _ocioInfo = new OCIOInformation();
    _ocioInfo->config = nullptr;
#endif
}

} // namespace MaterialX
