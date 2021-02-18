//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLCOMPILER_H
#define MATERIALX_CODEGEN_OSLCOMPILER_H

/// @file OslCompiler.h
/// TODO: Docs

#include <MaterialXCodegen/FragmentCompiler.h>

namespace MaterialX
{
namespace Codegen
{

class OslCompiler : public FragmentCompiler
{
public:
    OslCompiler(const Context& context);

    /// Create a new instance of this compiler.
    static FragmentCompilerPtr create(const Context& context);

    void compileShader(const Output& output, SourceCode& result) const override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
