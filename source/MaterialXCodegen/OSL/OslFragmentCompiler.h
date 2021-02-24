//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLFRAGMENTCOMPILER_H
#define MATERIALX_CODEGEN_OSLFRAGMENTCOMPILER_H

/// @file OslFragmentCompiler.h
/// TODO: Docs

#include <MaterialXCodegen/FragmentCompiler.h>

namespace MaterialX
{
namespace Codegen
{

class OslFragmentCompiler : public FragmentCompiler
{
public:
    OslFragmentCompiler(const Context& context);

    /// Create a new instance of this compiler.
    static FragmentCompilerPtr create(const Context& context);

    void compileShader(const Output& output, SourceCode& result) const override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
