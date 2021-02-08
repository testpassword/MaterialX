//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CONTEXT_H
#define MATERIALX_CODEGEN_CONTEXT_H

/// @file Context.h
/// TODO: Docs

#include <MaterialXCodegen/Library.h>
#include <MaterialXRuntime/RtToken.h>

namespace MaterialX
{
namespace Codegen
{

/// @class CodegenOptions
/// Base class for options used by a code generator.
class Options : public RtSharedBase<Options>
{
public:
    /// If true the y-component of texture coordinates used for sampling
    /// file textures will be flipped before sampling. This can be used if
    /// file textures need to be flipped vertically to match the target's
    /// texture space convention. By default this option is false.
    bool fileTextureVerticalFlip = false;
};

/// @class Context
/// Base class for context data needed by fragment generators.
/// Derived generators can derive from this class to hold
/// custom context data and needed by the generator.
class Context : public RtSharedBase<Context>
{
public:
    /// Destructor.
    virtual ~Context();

    /// Return an instance to the code generator that created this context.
    const FragmentGenerator* getGenerator() const
    {
        return _generator.get();
    }

    /// Return the options instance.
    const Options* getOptions() const
    {
        return _options.get();
    }

    /// Return the options instance.
    Options* getOptions()
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
    Context(FragmentGeneratorPtr generator, OptionsPtr options);

    // Fragment generator.
    FragmentGeneratorPtr _generator;

    // Options.
    OptionsPtr _options;

    // Set of globally reserved words.
    StringSet _reservedWords;

    // Map of string substitutions used by codegen.
    StringMap _substitutions;
};

} // namepspace Codegen
} // namepspace MaterialX


#endif
