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

    virtual void compileShader(const Fragment::Output& output, SourceCode& result) const = 0;

    virtual void declareVariable(const Fragment::Port& port, bool assignDefault, SourceCode& result) const;
    virtual void emitBlock(const string& block, SourceCode& result) const;
    virtual void emitInclude(const FilePath& file, SourceCode& result) const;
    virtual void emitVariable(const Fragment::Input& input, SourceCode& result) const;
    virtual void emitTypeDefinitions(SourceCode& result) const;

protected:
    const Context& _context;
};

} // namespace Codegen
} // namespace MaterialX

#endif
