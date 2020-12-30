//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTCODEGENCONTEXT_H
#define MATERIALX_RTCODEGENCONTEXT_H

/// @file RtCodegenContext.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>

#include <MaterialXGenShader/Syntax.h>

namespace MaterialX
{

/// @class RtCodegenOptions
/// Base class for options used by a code generator.
class RtCodegenOptions : public RtSharedBase<RtCodegenOptions>
{
public:
    /// If true the y-component of texture coordinates used for sampling
    /// file textures will be flipped before sampling. This can be used if
    /// file textures need to be flipped vertically to match the target's
    /// texture space convention. By default this option is false.
    bool fileTextureVerticalFlip = false;
};

using RtCodegenOptionsPtr = RtSharedPtr<RtCodegenOptions>;


/// @class RtCodegenContext
/// Base class for context data needed by code generators.
/// Derived code generators can derive from this class to hold
/// custom context data and needed by the generator.
class RtCodegenContext : public RtSharedBase<RtCodegenContext>
{
public:
    /// Destructor.
    virtual ~RtCodegenContext() {}

    /// Return an instance to the code generator that created this context.
    const RtCodeGenerator* getCodeGenerator() const
    {
        return _generator.get();
    }

    /// Return the options instance.
    const RtCodegenOptions* getOptions() const
    {
        return _options.get();
    }

    /// Return the options instance.
    RtCodegenOptions* getOptions()
    {
        return _options.get();
    }

    /// Add reserved words that should not be used as
    /// identifiers during code generation.
    void addReservedWords(const StringSet& names)
    {
        _reservedWords.insert(names.begin(), names.end());
    }

    /// Return the set of reserved words that should not be used
    /// as identifiers during code generation.
    const StringSet& getReservedWords() const
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
    RtCodegenContext(RtCodeGeneratorConstPtr generator, RtCodegenOptionsPtr options) :
        _generator(generator),
        _options(options)
    {}

    // Code generator for this context.
    RtCodeGeneratorConstPtr _generator;

    // Options.
    RtCodegenOptionsPtr _options;

    // Set of globally reserved words.
    StringSet _reservedWords;

    // Map of string substitutions used by codegen.
    StringMap _substitutions;
};

/// @class RtOslContext
/// Class for context data needed by OSL code generators.
class RtOslContext : public RtCodegenContext
{
public:
    /// Constructor.
    RtOslContext(RtCodeGeneratorConstPtr generator, RtCodegenOptionsPtr options);
};

}

#endif
