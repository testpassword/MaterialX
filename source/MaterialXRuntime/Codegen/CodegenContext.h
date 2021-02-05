//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CODEGENCONTEXT_H
#define MATERIALX_CODEGEN_CODEGENCONTEXT_H

/// @file CodegenContext.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>

#include <MaterialXGenShader/Syntax.h>

namespace MaterialX
{
namespace Codegen
{

/// @class CodegenOptions
/// Base class for options used by a code generator.
class CodegenOptions : public RtSharedBase<CodegenOptions>
{
public:
    /// If true the y-component of texture coordinates used for sampling
    /// file textures will be flipped before sampling. This can be used if
    /// file textures need to be flipped vertically to match the target's
    /// texture space convention. By default this option is false.
    bool fileTextureVerticalFlip = false;
};

/// @class CodegenContext
/// Base class for context data needed by code generators.
/// Derived code generators can derive from this class to hold
/// custom context data and needed by the generator.
class CodegenContext : public RtSharedBase<CodegenContext>
{
public:
    /// Destructor.
    virtual ~CodegenContext() {}

    /// Return an instance to the code generator that created this context.
    const CodeGenerator* getCodeGenerator() const
    {
        return _generator.get();
    }

    /// Return the options instance.
    const CodegenOptions* getOptions() const
    {
        return _options.get();
    }

    /// Return the options instance.
    CodegenOptions* getOptions()
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
    CodegenContext(CodeGeneratorPtr generator, CodegenOptionsPtr options) :
        _generator(generator),
        _options(options)
    {}

    // Code generator for this context.
    CodeGeneratorPtr _generator;

    // Options.
    CodegenOptionsPtr _options;

    // Set of globally reserved words.
    StringSet _reservedWords;

    // Map of string substitutions used by codegen.
    StringMap _substitutions;
};

/// @class OslContext
/// Class for context data needed by OSL code generators.
class OslContext : public CodegenContext
{
public:
    /// Constructor.
    OslContext(CodeGeneratorPtr generator, CodegenOptionsPtr options);
};

} // namepspace Codegen
} // namepspace MaterialX


#endif
