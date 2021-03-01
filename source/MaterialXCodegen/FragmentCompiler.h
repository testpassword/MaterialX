//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_FRAGMENTCOMPILER_H
#define MATERIALX_CODEGEN_FRAGMENTCOMPILER_H

/// @file FragmentCompiler.h
/// TODO: Docs

#include <MaterialXCodegen/Fragment.h>

#include <MaterialXFormat/File.h>

namespace MaterialX
{
namespace Codegen
{

/// @class FragmentCompiler
/// Class compiling fragments into functions and shader source code.
class FragmentCompiler : public RtSharedBase<FragmentCompiler>
{
public:
    FragmentCompiler(const Context& context);
    virtual ~FragmentCompiler() {}

    virtual void compileShader(const Output& output, SourceCode& result) const = 0;

    virtual void declareVariable(const Attribute& attr, bool assignDefault, SourceCode& result) const;
    virtual void emitBlock(const string& block, SourceCode& result) const;
    virtual void emitInclude(const FilePath& file, SourceCode& result) const;
    virtual void emitTypeDefinitions(SourceCode& result) const;
    virtual void emitFunctionDefinitions(const FragmentGraph& graph, SourceCode& result) const;
    virtual void emitFunctionCalls(const FragmentGraph& graph, SourceCode& result) const;

    string getResult(const Input& input) const;

    /// Return the set of reserved words that should not be used
    /// as identifiers during code generation.
    const RtTokenSet& getReservedWords() const
    {
        return _reservedWords;
    }

protected:
    const Context& _context;

    // Set of globally reserved words.
    RtTokenSet _reservedWords;

    // Map of string substitutions used by the compiler.
    StringMap _substitutions;
};

} // namespace Codegen
} // namespace MaterialX

#endif
