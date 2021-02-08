//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLGENERATOR_H
#define MATERIALX_CODEGEN_OSLGENERATOR_H

/// @file OslGenerator.h
/// TODO: Docs

#include <MaterialXCodegen/FragmentGenerator.h>

namespace MaterialX
{
namespace Codegen
{

/// @class OslGenerator
/// Fragment generator for vanilla OSL.
class OslGenerator : public FragmentGenerator
{
  public:
    /// Target identifier constant for this generator.
    static const RtToken TARGET;

  public:
    /// Constructor.
    OslGenerator(OptionsPtr options);
      
    /// Create a new instance of this code generator.
    static FragmentGeneratorPtr create(OptionsPtr options);

    /// Return the target identifier for this generator.
    const RtToken& getTarget() const override;

    /// Return the syntax object for this generator.
    const Codegen::Syntax& getSyntax() const override;

    /// Create a fragment from the given node or nodegraph.
//    FragmentPtr createFragment(const RtNode& node) const override;

    /// Create a fragment graph from the given node or nodegraph.
//    FragmentPtr createFragmentGraph(const RtNode& node) const override;

  protected:
    SyntaxPtr _syntax;
};

} // namespace Codegen
} // namespace MaterialX

#endif
