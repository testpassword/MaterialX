//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_OCIO_COLOR_MANAGEMENT_SYSTEM_H
#define MATERIALX_OCIO_COLOR_MANAGEMENT_SYSTEM_H

/// @file
/// Color management system which used the OCIO library

#include <MaterialXFormat/File.h>
#include <MaterialXGenShader/Nodes/SourceCodeNode.h>
#include <MaterialXGenShader/ColorManagementSystem.h>

namespace MaterialX
{

class OCIOInformation;

/// A shared pointer to a OCIOColorManagementSystem
using OCIOColorManagementSystemPtr = shared_ptr<class OCIOColorManagementSystem>;

/// @class OCIOColorManagementSystem
/// Class for a color management system which uses OCIO.
///
class MX_GENSHADER_API OCIOColorManagementSystem : public ColorManagementSystem
{
  public:
    virtual ~OCIOColorManagementSystem() { }

    /// Create a new OCIOColorManagementSystem
    static OCIOColorManagementSystemPtr create(const string& target);

    void loadLibrary(DocumentPtr document) override;

    /// Returns whether this color management system supports a provided transform
    bool supportsTransform(const ColorSpaceTransform& transform) const override;

    /// Create a node to use to perform the given color space transformation.
    ShaderNodePtr createNode(const ShaderGraph* parent, const ColorSpaceTransform& transform, const string& name,
                             GenContext& context) const override;

    /// Set the current configuration file to use
    bool setConfigFile(const FilePath& configFile);

    const FilePath& getConfigFile() const
    {
        return _configFile;
    }

    /// Return the OCIOColorManagementSystem name
    const string& getName() const override
    {
        return OCIOColorManagementSystem::CMS_NAME;
    }

    static const string CMS_NAME;

  protected:
    /// Returns an implementation for a given transform
    ImplementationPtr getImplementation(const ColorSpaceTransform& transform) const override;

    /// Protected constructor
    OCIOColorManagementSystem(const string& target);

  private:
    FilePath _configFile;
    OCIOInformation*  _ocioInfo;
    StringSet _colorSpaceNames;

    string _target;
    bool _targetSupported;
};


/// Extending the SourceCodeNode with requirements for OCIO.
class MX_GENSHADER_API OCIOSourceCodeNode : public SourceCodeNode
{
public:
    // TODO: Add more methods as needed...
    static ShaderNodeImplPtr create();

    void emitFunctionDefinition(const ShaderNode& node, GenContext& context, ShaderStage& stage) const override;
    void emitFunctionCall(const ShaderNode& node, GenContext& context, ShaderStage& stage) const override;
};


} // namespace MaterialX

#endif
