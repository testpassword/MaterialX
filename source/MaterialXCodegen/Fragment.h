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

#include <MaterialXCodegen/Attribute.h>
#include <MaterialXCodegen/SourceCode.h>

namespace MaterialX
{
namespace Codegen
{

/// Fragment types.
enum FragmentType
{
    FRAGMENT_TYPE_SOURCE_CODE,
    FRAGMENT_TYPE_GRAPH,
    FRAGMENT_TYPE_BSDF,
    FRAGMENT_TYPE_SHADER,
    FRAGMENT_TYPE_LAST
};

/// Flags for classifying fragments into different categories.
class FragmentClassification
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

/// A fragment creator function type.
using FragmentCreatorFunction = std::function<FragmentPtr(const RtToken&)>;

/// @class Fragment
/// Class holding a fragment of code.
class Fragment : public RtSharedBase<Fragment>, public NamedObject
{
  public:
    /// Constructor.
    Fragment(const RtToken& name);

    /// Destructor.
    virtual ~Fragment() {}

    /// Return the fragment type.
    virtual FragmentType getType() const = 0;

    /// Return the fragment class name.
    virtual const RtToken& getClassName() const = 0;

    /// Create a copy of this fragment.
    virtual FragmentPtr clone() const = 0;

    /// Copy data from this fragment to another.
    /// Given fragment must be of the same type.
    virtual void copy(Fragment& other) const;

    FragmentGraph* getParent()
    {
        return _parent;
    }

    const FragmentGraph* getParent() const
    {
        return _parent;
    }

    void setParent(FragmentGraph* parent)
    {
        _parent = parent;
    }

    bool hasClassification(uint32_t mask) const
    {
        return (_classification & mask) != 0;
    }

    void setClassification(uint32_t mask, bool value = true)
    {
        _classification = value ? _classification | mask : _classification & ~mask;
    }

    uint32_t getClassificationMask() const
    {
        return _classification;
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

    virtual Input* createInput(const RtToken& name, const RtToken& type);

    virtual Output* createOutput(const RtToken& name, const RtToken& type);

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

    virtual void emitFunctionDefinitions(const Context& context, SourceCode& result) const = 0;

    virtual void emitFunctionCall(const Context& context, SourceCode& result) const = 0;

  protected:
    // Parent fragment graph.
    FragmentGraph* _parent;

    // Fragment classification mask.
    uint32_t _classification;

    // Fragment function name.
    RtToken _functionName;

    // Allocator for large values.
    RtAllocator _allocator;

    // Fragment inputs.
    InputList _inputs;

    // Fragment outputs.
    OutputList _outputs;
};

/// A container of fragments.
using FragmentList = ObjectList<Fragment>;


/// @class FragmentGraph
/// Class holding a set of fragments connected in a graph.
class FragmentGraph : public Fragment
{
public:
    /// Constructor.
    FragmentGraph(const RtToken& name);

    /// Create a new instance of this class.
    static FragmentPtr create(const RtToken& name);

    /// Return the class name for this fragment.
    static const RtToken& className();

    /// Create a copy of this fragment.
    FragmentPtr clone() const override;

    /// Copy data from this fragment to another.
    /// Given fragment must be of the same type.
    void copy(Fragment& other) const override;

    /// Return the fragment type.
    FragmentType getType() const override
    {
        return FRAGMENT_TYPE_GRAPH;
    }

    /// Return the fragment class name.
    const RtToken& getClassName() const override
    {
        return className();
    }

    /// Add a fragment to the graph and make
    /// this graph parent of the fragment.
    void addFragment(FragmentPtr fragment);

    /// Remove a fragment from the graph and return it.
    FragmentPtr removeFragment(const RtToken& name);

    /// Return the number of fragments contained in this graph.
    size_t numFragments() const;

    /// Return a fragment by index.
    Fragment* getFragment(size_t index) const;

    /// Return a fragment by name.
    Fragment* getFragment(const RtToken& name) const;

    /// Create a connections between two fragments.
    void connect(Output* src, Input* dst);

    /// Create a connections between two fragments.
    void connect(const RtToken& srcFragment, const RtToken& srcOutput,
                 const RtToken& dstFragment, const RtToken& dstOutput);

    Input* createInput(const RtToken& name, const RtToken& type) override;

    Output* createOutput(const RtToken& name, const RtToken& type) override;

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

    void emitFunctionDefinitions(const Context& context, SourceCode& result) const override;

    void emitFunctionCall(const Context& context, SourceCode& result) const override;

protected:
    // Child fragments.
    FragmentList _fragments;

    // Graph internal input sockets.
    OutputList _inputSockets;

    // Graph internal output sockets.
    InputList _outputSockets;
};

/// @class SourceFragment
/// Class holding a fragment of static source code.
class SourceFragment : public Fragment
{
public:
    /// Constructor.
    SourceFragment(const RtToken& name);

    /// Create a new instance of this class.
    static FragmentPtr create(const RtToken& name);

    /// Return the class name for this fragment.
    static const RtToken& className();

    /// Create a copy of this fragment.
    FragmentPtr clone() const override;

    /// Copy data from this fragment to another.
    /// Given fragment must be of the same type.
    void copy(Fragment& other) const override;

    /// Return the fragment type.
    FragmentType getType() const override
    {
        return FRAGMENT_TYPE_SOURCE_CODE;
    }

    /// Return the fragment class name.
    const RtToken& getClassName() const override
    {
        return className();
    }

    /// Set fragment source code.
    void setSourceCode(const string* sourceCode)
    {
        _sourceCode = sourceCode;
    }

    bool isInline() const
    {
        return _functionName == EMPTY_TOKEN;
    }

    void emitFunctionDefinitions(const Context& context, SourceCode& result) const override;

    void emitFunctionCall(const Context& context, SourceCode& result) const override;

protected:
    /// Source code for this fragment.
    const string* _sourceCode;
};

} // namespace Codegen
} // namespace MaterialX

#endif
