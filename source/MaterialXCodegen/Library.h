//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_LIBRARY_H
#define MATERIALX_CODEGEN_LIBRARY_H

/// @file
/// Library-wide includes and types.  This file should be the first include for
/// any public header in the MaterialXCodegen library.

#include <MaterialXRuntime/Library.h>

namespace MaterialX
{
namespace Codegen
{

class Syntax;
class TypeSyntax;
class Options;
class Context;
class FragmentGenerator;
class FragmentCompiler;
class Fragment;
class FragmentGraph;
class SourceFragment;

using SyntaxPtr = RtSharedPtr<Syntax>;
using TypeSyntaxPtr = RtSharedPtr<TypeSyntax>;
using OptionsPtr = RtSharedPtr<Options>;
using ContextPtr = RtSharedPtr<Context>;
using FragmentGeneratorPtr = RtSharedPtr<FragmentGenerator>;
using FragmentCompilerPtr = RtSharedPtr<FragmentCompiler>;
using FragmentPtr = RtSharedPtr<Fragment>;

} // namespace Codegen
} // namespace MaterialX

#endif
