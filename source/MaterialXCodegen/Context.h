//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CONTEXT_H
#define MATERIALX_CODEGEN_CONTEXT_H

/// @file Context.h
/// TODO: Docs

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/ColorManagement.h>

#include <MaterialXRuntime/RtToken.h>

namespace MaterialX
{
namespace Codegen
{

/// Method to use for directional albedo evaluation
enum DirectionalAlbedoMethod
{
    /// Use a curve fit approximation for directional albedo.
    DIRECTIONAL_ALBEDO_CURVE_FIT,

    /// Use a table look-up for directional albedo.
    DIRECTIONAL_ALBEDO_TABLE,

    /// Use importance sampling for directional albedo.
    DIRECTIONAL_ALBEDO_IS
};

/// @class CodegenOptions
/// Base class for options used by a code generator.
class Options : public RtSharedBase<Options>
{
public:
    Options();

    /// Create a new options instance.
    static OptionsPtr create();

    /// If true the y-component of texture coordinates used for sampling
    /// file textures will be flipped before sampling. This can be used if
    /// file textures need to be flipped vertically to match the target's
    /// texture space convention. By default this option is false.
    bool fileTextureVerticalFlip = false;

    /// An optional override for the target color space.
    /// Shader fragments will be generated to transform
    /// input values and textures into this color space.
    RtToken targetColorSpaceOverride;

    /// Sets the method to use for directional albedo evaluation
    /// for HW shader targets.
    DirectionalAlbedoMethod directionalAlbedoMethod;
};

/// @class Context
/// Base class for context data needed for codegen to a specific target.
class Context : public RtSharedBase<Context>
{
public:
    /// Destructor.
    virtual ~Context();

    /// Return the target identifier.
    virtual const RtToken& getTarget() const = 0;

    /// Return the syntax object for this target.
    virtual const Syntax& getSyntax() const = 0;

    /// Get the fragment generator for this target.
    virtual const FragmentGenerator& getGenerator() const = 0;

    /// Get the fragment compiler for this target.
    virtual const FragmentCompiler& getCompiler() const = 0;

    /// Return the options instance.
    const Options& getOptions() const
    {
        return *_options;
    }

    /// Return the options instance.
    Options& getOptions()
    {
        return *_options;
    }

    void setColorManagementSystem(ColorManagementSystemPtr cms)
    {
        _cms = cms;
    }

    const ColorManagementSystem* getColorManagementSystem() const
    {
        return _cms.get();
    }

protected:
    /// Constructor.
    Context(const OptionsPtr& options);

    // Options.
    OptionsPtr _options;

    // Color management system.
    ColorManagementSystemPtr _cms;
};

} // namespace Codegen
} // namespace MaterialX


#endif
