//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_FRAGMENTGENERATOR_H
#define MATERIALX_CODEGEN_FRAGMENTGENERATOR_H

/// @file CodeGenerator.h
/// TODO: Docs

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

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

    /// Create a fragment instance of the given class with the given name.
    virtual FragmentPtr createFragment(const RtToken& className, const RtToken& instanceName) const;

    /// Create a fragment from the given node or nodegraph.
    /// If multiple fragments are created the root fragment is returned.
    /// All fragments are added as children to the given parent graph.
    virtual FragmentPtr createFragment(const RtNode& node, FragmentGraph& parent) const;

    /// Create a fragment graph from the given node or nodegraph.
    /// If a node is given a graph will be created by traversing 
    /// all upstream dependencies.
    virtual FragmentPtr createFragmentGraph(const RtNode& node, bool publishAllInputs = false) const;

    /// Create and connect any additional fragments needed for this node.
    /// If such sub-fragments are needed a new fragment graph is created that
    /// encapsulates this network, and this fragment graph is returned.
    /// If no sub-fragments are needed the main fragment passed in is returned.
    virtual FragmentPtr createSubFragments(const RtNode& node, Fragment& fragment) const;

    /// Create a mask with classifications matching the given node.
    virtual uint32_t getClassificationMask(const RtNode& node) const;

    /// Register a creator function for a fragment class.
    void registerFragmentClass(const RtToken& className, FragmentCreatorFunction creator);

  protected:
    /// Constructor.
    FragmentGenerator(const Context& context);

  protected:
    const Context& _context;

    /// Creator functions for registered fragment classes.
    RtTokenMap<FragmentCreatorFunction> _creatorFunctions;
};

} // namespace Codegen
} // namespace MaterialX

#endif
