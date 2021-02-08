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

    /// Return the target identifier for this generator.
    virtual const RtToken& getTarget() const = 0;

    /// Return the syntax object for this generator.
    virtual const Syntax& getSyntax() const = 0;

    /// Return the options object for this generator.
    const Options& getOptions() const
    {
        return *_options;
    };

    /// Return the options object for this generator.
    Options& getOptions()
    {
        return *_options;
    };

    /// Create an empty fragment.
    virtual FragmentPtr createFragment(const RtToken& name) const;

    /// Create a fragment from the given node or nodegraph.
    virtual FragmentPtr createFragment(const RtNode& node) const;

    /// Create a fragment graph from the given node or nodegraph.
    virtual FragmentGraphPtr createFragmentGraph(const RtNode& node) const;

  protected:
    /// Constructor.
    FragmentGenerator(OptionsPtr options);

    OptionsPtr _options;
};

} // namespace Codegen
} // namespace MaterialX

#endif
