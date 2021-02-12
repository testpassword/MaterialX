//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLGENERATOR_H
#define MATERIALX_CODEGEN_OSLGENERATOR_H

/// @file OslGenerator.h
/// TODO: Docs

#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/FragmentCompiler.h>

namespace MaterialX
{
namespace Codegen
{

/// @class OslGenerator
/// Fragment generator for vanilla OSL.
class OslGenerator : public FragmentGenerator
{
  public:
    /// Constructor.
    OslGenerator(const Context& context);

    /// Create a new instance of this generator.
    static FragmentGeneratorPtr create(const Context& context);
};

} // namespace Codegen
} // namespace MaterialX

#endif
