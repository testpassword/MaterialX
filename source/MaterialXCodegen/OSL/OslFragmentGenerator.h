//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLFRAGMENTGENERATOR_H
#define MATERIALX_CODEGEN_OSLFRAGMENTGENERATOR_H

/// @file OslFragmentGenerator.h
/// TODO: Docs

#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/FragmentCompiler.h>

namespace MaterialX
{
namespace Codegen
{

/// @class OslFragmentGenerator
/// Fragment generator for vanilla OSL.
class OslFragmentGenerator : public FragmentGenerator
{
  public:
    /// Constructor.
    OslFragmentGenerator(const Context& context);

    /// Create a new instance of this generator.
    static FragmentGeneratorPtr create(const Context& context);
};

} // namespace Codegen
} // namespace MaterialX

#endif
