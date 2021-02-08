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

    /// Constructor.
    Fragment(const RtToken& name);

    /// Create a new fragment.
    static FragmentPtr create(const RtToken& name);

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
        return _inputs.size();
    }
    Output* getOutput(size_t index) const
    {
        return _outputs[index];
    }

    /// Set fragment source code.
    void setSourceCode(const string& source);

    /// Return fragment source code.
    const string& getSourceCode() const
    {
        return _sourceCode;
    }

    /// Return include file dependencies.
    const StringSet& getIncludes() const
    {
        return _includes;
    }

protected:
    /// Name of the fragment.
    const RtToken _name;

    /// Source code for this fragment.
    string _sourceCode;

    /// Set of include files that has been included.
    StringSet _includes;

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

} // namepspace Codegen
} // namepspace MaterialX

#endif
