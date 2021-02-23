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

/// Macro declaring required methods and mambers on a fragment class.
#define DECLARE_FRAGMENT_CLASS(T)                                        \
public:                                                                  \
    T(const RtToken& name);                                              \
    static FragmentPtr create(const RtToken& name);                      \
    static const RtToken& className();                                   \
    FragmentPtr clone() const override;                                  \
    const RtToken& getClassName() const override { return className(); } \

/// Macro defining required methods and mambers on a fragment class.
/// The constructor has to be defined separatelly.
#define DEFINE_FRAGMENT_CLASS_NO_CONSTRUCT(T)                            \
    FragmentPtr T::create(const RtToken& name)                           \
    {                                                                    \
        return FragmentPtr(new T(name));                                 \
    }                                                                    \
    const RtToken& T::className()                                        \
    {                                                                    \
        static const RtToken CLASS_NAME(#T);                             \
        return CLASS_NAME;                                               \
    }                                                                    \
    FragmentPtr T::clone() const                                         \
    {                                                                    \
        FragmentPtr other = T::create(_name);                            \
        other->copy(*this);                                              \
        return other;                                                    \
    }                                                                    \

/// Macro defining required methods and mambers on a fragment class
/// including a default constructor.
#define DEFINE_FRAGMENT_CLASS(T, BaseClass)                              \
    T::T(const RtToken& name) : BaseClass(name) {}                       \
    DEFINE_FRAGMENT_CLASS_NO_CONSTRUCT(T)                                \

namespace MaterialX
{
namespace Codegen
{

/// Flags for classifying fragments into different categories.
class FragmentClassification
{
public:
    // Node classes
    static const uint32_t TEXTURE = 1 << 0;  /// A fragment that outputs floats, colors, vectors, etc.
    static const uint32_t CLOSURE = 1 << 1;  /// A fragment that represents light integration
    static const uint32_t SHADER = 1 << 2;  /// A fragment that outputs a shader
    // Specific texture node types
    static const uint32_t FILETEXTURE = 1 << 3;  /// A file texture
    static const uint32_t CONDITIONAL = 1 << 4;  /// A conditional
    static const uint32_t CONSTANT = 1 << 5;  /// A constant
    // Specific closure types
    static const uint32_t BSDF = 1 << 6;  /// A BSDF
    static const uint32_t BSDF_R = 1 << 7;  /// A BSDF only for reflection
    static const uint32_t BSDF_T = 1 << 8;  /// A BSDF only for transmission
    static const uint32_t EDF = 1 << 9;  /// An EDF
    static const uint32_t VDF = 1 << 10; /// A VDF
    static const uint32_t LAYER = 1 << 11; /// A fragment for vertical layering of other closure fragments
    static const uint32_t THINFILM = 1 << 12; /// A fragment for adding thin-film over microfacet BSDFs
    // Specific shader types
    static const uint32_t SURFACE = 1 << 13; /// A surface shader fragment
    static const uint32_t VOLUME = 1 << 14; /// A volume shader fragment
    static const uint32_t LIGHT = 1 << 15; /// A light shader fragment
    // Specific conditional types
    static const uint32_t IFELSE = 1 << 16; /// An if-else statement
    static const uint32_t SWITCH = 1 << 17; /// A switch statement
    // Types based on nodegroup
    static const uint32_t SAMPLE2D = 1 << 18; /// A function that can be sampled in 2D (uv space)
    static const uint32_t SAMPLE3D = 1 << 19; /// A function that can be sampled in 3D (position)
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

    /// Return the fragment class name.
    virtual const RtToken& getClassName() const = 0;

    /// Create a copy of this fragment.
    virtual FragmentPtr clone() const = 0;

    /// Copy data from another fragment.
    /// Given fragment must be of the same type.
    virtual void copy(const Fragment& other);

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
    bool isA()
    {
        return dynamic_cast<T*>(this) != nullptr;
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

    /// Create an input.
    virtual Input* createInput(const RtToken& name, const RtToken& type);

    /// Create an output.
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

    virtual void finalize(const Context& context);

    virtual void emitFunctionDefinitions(const Context& context, SourceCode& result) const;

    virtual void emitFunctionCall(const Context& context, SourceCode& result) const;

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
    DECLARE_FRAGMENT_CLASS(FragmentGraph)

public:
    /// Copy data from this fragment to another.
    /// Given fragment must be of the same type.
    void copy(const Fragment& other) override;

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

    /// Create a connection between two fragments.
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

    /// Finalize the fragment graph construction.
    void finalize(const Context& context) override;

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
/// A fragment of static source code.
class SourceFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(SourceFragment)

public:
    /// Copy data from another fragment.
    /// Given fragment must be of the same type.
    void copy(const Fragment& other) override;

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
