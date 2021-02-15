//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_FRAGMENTGENERATOR_H
#define MATERIALX_CODEGEN_FRAGMENTGENERATOR_H

/// @file CodeGenerator.h
/// TODO: Docs

#include <MaterialXCodegen/Library.h>

#include <MaterialXRuntime/RtToken.h>

namespace MaterialX
{
namespace Codegen
{

/// @class FragmentGenerator
/// Abstract base class for fragment generators.
class FragmentGenerator : public RtSharedBase<FragmentGenerator>
{
  public:
    /// Destructor.
    virtual ~FragmentGenerator() {}

    /// Create a fragment from the given node or nodegraph.
    virtual FragmentPtr createFragment(const RtNode& node) const;

    /// Create a fragment graph from the given node or nodegraph.
    /// If a node is given a graph will be created by traversing 
    /// all upstream dependencies.
    virtual FragmentPtr createFragmentGraph(const RtNode& node, bool publishAllInputs = false) const;

  protected:
    /// Constructor.
    FragmentGenerator(const Context& context);

    virtual uint32_t getClassMask(const RtNode& node) const;

    const Context& _context;
};

} // namespace Codegen
} // namespace MaterialX

#endif
