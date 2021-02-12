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

    /// Create a fragment generator for this target.
    virtual FragmentGeneratorPtr createGenerator() const = 0;

    /// Create a fragment compiler for this target.
    virtual FragmentCompilerPtr createCompiler() const = 0;

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

    /// Return the set of reserved words that should not be used
    /// as identifiers during code generation.
    const RtTokenSet& getReservedWords() const
    {
        return _reservedWords;
    }

    /// Return the map of string substitutions used in codegen.
    const StringMap& getSubstitutions() const
    {
        return _substitutions;
    }

protected:
    /// Constructor.
    Context(const OptionsPtr& options);

    // Options.
    OptionsPtr _options;

    // Set of globally reserved words.
    RtTokenSet _reservedWords;

    // Map of string substitutions used by codegen.
    StringMap _substitutions;
};

} // namespace Codegen
} // namespace MaterialX


#endif
