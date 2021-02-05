//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CODEGENERATOR_H
#define MATERIALX_CODEGEN_CODEGENERATOR_H

/// @file CodeGenerator.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>
#include <MaterialXRuntime/Codegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

/// @class CodegenResult
/// Class holding the result returned by code generation.
class CodegenResult : public RtSharedBase<CodegenResult>
{
public:
    /// Create a new instance of this class.
    static CodegenResultPtr create();

    /// Add a new fragment to the results.
    void addFragment(const FragmentPtr& fragment);

    /// Remove a fragment from the results.
    void removeFragment(const RtToken& name);

    /// Return the number of fragments contained in this result.
    size_t numFragments() const;

    /// Return a fragment by index.
    FragmentPtr getFragment(size_t index) const;

    /// Return a fragment by name.
    FragmentPtr getFragment(const RtToken& name) const;

protected:
    vector<FragmentPtr> _fragmentsOrder;
    RtTokenMap<FragmentPtr> _fragments;
};


/// @class CodeGenerator
/// Abstract base class for code generators.
class CodeGenerator : public RtSharedBase<CodeGenerator>
{
public:
    /// Destructor.
    virtual ~CodeGenerator() {}

    /// Return the target identifier for this generator.
    virtual const RtToken& getTarget() const = 0;

    /// Return the syntax object for this generator.
    virtual const Syntax& getSyntax() const = 0;

    /// Create a new options instance for this generator.
    virtual CodegenOptionsPtr createOptions() const;

    /// Create a new context instance for this generator.
    virtual CodegenContextPtr createContext(CodegenOptionsPtr options) = 0;

    /// Create a fragment from the given node or nodegraph.
    virtual FragmentPtr createFragment(const RtNode& node) const;

    /// Create a fragment graph from the given node or nodegraph.
    virtual FragmentPtr createFragmentGraph(const RtNode& node) const;

    /// Generate code for the given prim and path.
    virtual CodegenResultPtr generate(const RtPrim& prim, const RtPath& path, CodegenContextPtr context) const = 0;

protected:
    /// Constructor.
    CodeGenerator() {}
};


/// @class OslGenerator
/// Code generator for vanilla OSL.
class OslGenerator : public CodeGenerator
{
public:
    /// Create a new instance of this code generator.
    static CodeGeneratorPtr create();

    /// Return the target identifier for this generator.
    const RtToken& getTarget() const override;

    /// Return the syntax object for this generator.
    const Codegen::Syntax& getSyntax() const override;

    /// Create a new context instance for this generator.
    CodegenContextPtr createContext(CodegenOptionsPtr options) override;

    /// Generate code for the given prim and path.
    CodegenResultPtr generate(const RtPrim& prim, const RtPath& path, CodegenContextPtr context) const override;

    /// Target identifier constant for this generator.
    static const RtToken TARGET;

protected:
    /// Constructor.
    OslGenerator();

    SyntaxPtr _syntax;
};

} // namepspace Codegen
} // namepspace MaterialX

#endif
