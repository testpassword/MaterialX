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

/// Flags for classifying fragments into different categories.
class FragmentClass
{
public:
    // Node classes
    static const uint32_t TEXTURE = 1 << 0;  /// Any node that outputs floats, colors, vectors, etc.
    static const uint32_t CLOSURE = 1 << 1;  /// Any node that represents light integration
    static const uint32_t SHADER = 1 << 2;  /// Any node that outputs a shader
    // Specific texture node types
    static const uint32_t FILETEXTURE = 1 << 3;  /// A file texture node
    static const uint32_t CONDITIONAL = 1 << 4;  /// A conditional node
    static const uint32_t CONSTANT = 1 << 5;  /// A constant node
    // Specific closure types
    static const uint32_t BSDF = 1 << 6;  /// A BSDF node
    static const uint32_t BSDF_R = 1 << 7;  /// A BSDF node only for reflection
    static const uint32_t BSDF_T = 1 << 8;  /// A BSDF node only for transmission
    static const uint32_t EDF = 1 << 9;  /// A EDF node
    static const uint32_t VDF = 1 << 10; /// A VDF node
    static const uint32_t LAYER = 1 << 11; /// A node for vertical layering of other closure nodes
    static const uint32_t THINFILM = 1 << 12; /// A node for adding thin-film over microfacet BSDF nodes
    // Specific shader types
    static const uint32_t SURFACE = 1 << 13; /// A surface shader node
    static const uint32_t VOLUME = 1 << 14; /// A volume shader node
    static const uint32_t LIGHT = 1 << 15; /// A light shader node
    // Specific conditional types
    static const uint32_t IFELSE = 1 << 16; /// An if-else statement
    static const uint32_t SWITCH = 1 << 17; /// A switch statement
    // Types based on nodegroup
    static const uint32_t SAMPLE2D = 1 << 18; /// Can be sampled in 2D (uv space)
    static const uint32_t SAMPLE3D = 1 << 19; /// Can be sampled in 3D (position)
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

        string getLongName() const
        {
            return parent->getName().str() + "_" + name.str();
        }
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

    bool isClass(uint32_t mask) const
    {
        return (_class & mask) != 0;
    }

    uint32_t getClassMask() const
    {
        return _class;
    }

    void setClass(uint32_t mask, bool value = true)
    {
        _class = value ? _class | mask : _class & ~mask;
    }

    template<class T>
    T* asA()
    {
        return static_cast<T*>(this);
    }

    template<class T>
    const T* asA() const
    {
        return static_cast<T*>(this);
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

    /// Fragment class flags.
    uint32_t _class;

    /// Fragment function name.
    RtToken _functionName;

    /// Source code for this fragment.
    string _sourceCode;

    /// Allocator for large values.
    RtAllocator _allocator;

    /// Fragment ports.
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

    /// Finalize the graph construction.
    void finalize(const Context& context, bool publishAllInputs = false);

    /// Compile the graph and cache the resulting source code.
    void precompile(const FragmentCompiler& compiler);

protected:
    ObjectList<Fragment> _fragments;
    ObjectList<Output> _inputSockets;
    ObjectList<Input> _outputSockets;
};

} // namespace Codegen
} // namespace MaterialX

#endif
