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
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtGeomPropDef.h>

namespace MaterialX
{
namespace Codegen
{

/// @class FragmentGenerator
/// Base class for fragment generators.
/// A fragment generator is a factory turning prims to fragments
/// for use in code generation.
class FragmentGenerator : public RtSharedBase<FragmentGenerator>
{
  public:
    /// Destructor.
    virtual ~FragmentGenerator() {}

    /// Register a creator function for a fragment class.
    void registerFragmentClass(const RtToken& className, FragmentCreatorFunction creator);

    /// Create a fragment of the given class with the given name.
    virtual FragmentPtr createFragment(const RtToken& className, const RtToken& name) const;

    /// Create a fragment of the given nodedef with the given name.
    virtual FragmentPtr createFragment(const RtNodeDef& nodedef, const RtToken& name) const;

    /// Create a fragment of the given geomprop with the given name.
    virtual FragmentPtr createFragment(const RtGeomPropDef& geomprop, const RtToken& name) const;

    /// Create a fragment from the given node or nodegraph instance.
    /// If multiple fragments are created the root fragment is returned.
    /// All fragments are added as children to the given parent graph.
    virtual FragmentPtr createFragment(const RtNode& node, FragmentGraph& parent) const;

    /// Create a fragment graph from the given node or nodegraph.
    /// If a node is given a graph will be created by traversing 
    /// all upstream dependencies.
    virtual FragmentPtr createFragmentGraph(const RtNode& node) const;

    /// Create and connect any additional fragments needed for this node.
    /// If such sub-fragments are needed a new fragment graph is created that
    /// encapsulates this network, and this fragment graph is returned.
    /// If no sub-fragments are needed the main fragment passed in is returned.
    virtual FragmentPtr createSubFragments(const RtNode& node, Fragment& fragment) const;

    /// Create a mask with classifications matching the given nodedef.
    virtual uint32_t getClassificationMask(const RtNodeDef& nodedef) const;

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
