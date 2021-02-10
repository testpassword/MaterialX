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

template<class T>
class ObjectList
{
public:
    using ObjectPtr = RtSharedPtr<T>;

    size_t size() const
    {
        return _order.size();
    }

    T* get(const RtToken& name) const
    {
        auto it = _objects.find(name);
        return it != _objects.end() ? it->second.get() : nullptr;
    }

    T* get(size_t index) const
    {
        return index < _order.size() ? _order[index] : nullptr;
    }

    T** order()
    {
        return _order.data();
    }

    const T** order() const
    {
        return _order.data();
    }

    void add(const RtToken& name, const ObjectPtr& obj)
    {
        _objects[name] = obj;
        _order.push_back(obj.get());
    }

    void remove(const RtToken& name)
    {
        auto i = _objects.find(name);
        if (i != _objects.end())
        {
            const T* ptr = i->second.get();
            for (auto j = _order.begin(); j != _order.end(); ++j)
            {
                if ((*j) == ptr)
                {
                    _order.erase(j);
                    break;
                }
            }
            _objects.erase(i);
        }
    }

    void clear()
    {
        _objects.clear();
        _order.clear();
    }

private:
    RtTokenMap<ObjectPtr> _objects;
    vector<T*> _order;
};

enum FragmentType
{
    FRAGMENT_TYPE_FUNCTION,
    FRAGMENT_TYPE_INLINE,
    FRAGMENT_TYPE_GRAPH,
    FRAGMENT_TYPE_STAGE,
    FRAGMENT_TYPE_BSDF,
    FRAGMENT_TYPE_LAST
};

/// @class Fragment
/// Class holding a fragment of code.
class Fragment : public RtSharedBase<Fragment>
{
  public:
    struct Port
    {
        Fragment* parent;
        RtToken type;
        RtToken name;
        RtToken variable;
    };

    struct Output;

    struct Input : public Port
    {
        RtValue value;
        Output* connection;
    };

    struct Output : public Port
    {
        std::set<Input*> connections;
    };

    using OutputPtr = RtSharedPtr<Output>;
    using InputPtr = RtSharedPtr<Input>;

  public:
    /// Constructor.
    Fragment(const RtToken& name);

    /// Destructor.
    virtual ~Fragment() {}

    /// Return the fragment name.
    const RtToken& getName() const
    {
        return _name;
    }

    /// Return the type of this fragment.
    virtual FragmentType getType() const
    {
        return _functionName != EMPTY_TOKEN ? FRAGMENT_TYPE_FUNCTION : FRAGMENT_TYPE_INLINE;
    }

    virtual Input* createInput(const RtToken& type, const RtToken& name);

    virtual Output* createOutput(const RtToken& type, const RtToken& name);

    size_t numInputs() const
    {
        return _inputs.size();
    }
    
    Input* getInput(size_t index) const
    {
        return _inputs.get(index);
    }

    Input* getInput(const RtToken& name) const
    {
        return _inputs.get(name);
    }

    size_t numOutputs() const
    {
        return _outputs.size();
    }

    Output* getOutput(size_t index = 0) const
    {
        return _outputs.get(index);
    }

    Output* getOutput(const RtToken& name) const
    {
        return _outputs.get(name);
    }

    /// Set fragment function name.
    void setFunctionName(const RtToken& functionName)
    {
        _functionName = functionName;
    }

    /// Return fragment function name.
    const RtToken& getFunctionName() const
    {
        return _functionName;
    }

    /// Set fragment source code.
    void setSourceCode(const string& sourceCode)
    {
        _sourceCode = sourceCode;
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

    ObjectList<Input> _inputs;
    ObjectList<Output> _outputs;
};

/// @class FragmentGraph
/// Class holding a set of fragments connected in a graph.
class FragmentGraph : public Fragment
{
public:
    /// Constructor.
    FragmentGraph(const RtToken& name);

    /// Return the type of this fragment.
    FragmentType getType() const override
    {
        return FRAGMENT_TYPE_GRAPH;
    }

    /// Add a fragment to the graph.
    void addFragment(const FragmentPtr& fragment);

    /// Remove a fragment from the results.
    void removeFragment(const RtToken& name);

    /// Return the number of fragments contained in this graph.
    size_t numFragments() const;

    /// Return a fragment by index.
    Fragment* getFragment(size_t index) const;

    /// Return a fragment by name.
    Fragment* getFragment(const RtToken& name) const;

    /// Create a connections between two fragments.
    void connect(Fragment::Output* src, Fragment::Input* dst);

    /// Create a connections between two fragments.
    void connect(const RtToken& srcFragment, const RtToken& srcOutput,
                 const RtToken& dstFragment, const RtToken& dstOutput);

    Input* createInput(const RtToken& type, const RtToken& name) override;

    Output* createOutput(const RtToken& type, const RtToken& name) override;

    Output* getInputSocket(size_t index) const
    {
        return _inputSockets.get(index);
    }

    Output* getInputSocket(const RtToken& name) const
    {
        return _inputSockets.get(name);
    }

    Input* getOutputSocket(size_t index = 0) const
    {
        return _outputSockets.get(index);
    }

    Input* getOutputSocket(const RtToken& name) const
    {
        return _outputSockets.get(name);
    }

    /// Prepare the graph for compilation.
    void finalize();

protected:
    ObjectList<Fragment> _fragments;
    ObjectList<Output> _inputSockets;
    ObjectList<Input> _outputSockets;
};

} // namespace Codegen
} // namespace MaterialX

#endif
