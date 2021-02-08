//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_FRAGMENT_H
#define MATERIALX_CODEGEN_FRAGMENT_H

/// @file Fragment.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>
#include <MaterialXRuntime/RtValue.h>

#include <MaterialXCodegen/Syntax.h>

namespace MaterialX
{
namespace Codegen
{

/// @class Fragment
/// Class holding a fragment of code.
class Fragment : public RtSharedBase<Fragment>
{
  public:
    struct Output
    {
        Fragment* parent;
        RtToken type;
        RtToken name;
        RtToken variable;
    };

    struct Input : public Output
    {
        RtValue value;
        Output* connection;
    };

  public:
    /// Constructor.
    Fragment(const RtToken& name);

    /// Return the fragment name.
    const RtToken& getName() const
    {
        return _name;
    }

    Input* createInput(const RtToken& type, const RtToken& name);
    Output* createOutput(const RtToken& type, const RtToken& name);

    size_t numInputs() const
    {
        return _inputs.size();
    }
    Input* getInput(size_t index) const
    {
        return _inputs[index];
    }

    size_t numOutputs() const
    {
        return _outputs.size();
    }
    Output* getOutput(size_t index) const
    {
        return _outputs[index];
    }

    /// Set source code as a function.
    void setSourceCodeFunction(const RtToken& functionName, const string& sourceCode);

    /// Set source code as an inline expression.
    void setSourceCodeInline(const string& sourceCode);

    /// Return fragment source code.
    const RtToken& getFunctionName() const
    {
        return _functionName;
    }

    /// Return true if the fragment source code is an inline expression.
    bool isInline() const
    {
        return _functionName == EMPTY_TOKEN;
    }

    /// Return fragment source code.
    const string& getSourceCode() const
    {
        return _sourceCode;
    }

  protected:
    /// Fragment name.
    const RtToken _name;

    /// Fragment function name.
    RtToken _functionName;

    /// Source code for this fragment.
    string _sourceCode;

    /// Allocator for large values.
    RtAllocator _allocator;

    vector<Input*> _inputs;
    vector<Output*> _outputs;
};

/*
/// @class CodegenResult
/// Class holding the result returned by code generation.
class FragmentGraph : public Fragment
{
public:
    /// Create a new instance of this class.
    static FragmentGraphPtr create();

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
*/

} // namespace Codegen
} // namespace MaterialX

#endif
