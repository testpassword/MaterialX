//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_ATTRIBUTE_H
#define MATERIALX_CODEGEN_ATTRIBUTE_H

/// @file Attribute.h
/// TODO: Docs

#include <MaterialXCodegen/Library.h>

#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{
namespace Codegen
{

class Fragment;
class Input;
class Output;

/// @class Attribute
/// Base class for attributes on a fragment.
class Attribute : public NamedObject
{
  public:
    /// Constructor.
    Attribute(const RtToken& name, const RtToken& type) :
        NamedObject(name),
        _parent(nullptr),
        _type(type),
        _variable(name)
    {}

    /// Return the parent fragment.
    Fragment* getParent() const
    {
        return _parent;
    }

    /// Return the parent fragment.
    void setParent(Fragment* parent)
    {
        _parent = parent;
    }

    /// Return the attribute type.
    const RtToken& getType() const
    {
        return _type;
    }

    /// Set the attribute type.
    void setType(const RtToken& type)
    {
        _type = type;
    }

    /// Return the attribute variable name.
    const RtToken& getVariable() const
    {
        return _variable;
    }

    /// Set the attribute variable name.
    void setVariable(const RtToken& variable)
    {
        _variable = variable;
    }

    /// Return a longer name for the attribute,
    /// constructed by prepending the parent name.
    string getLongName() const;

  protected:
    // The parent fragment.
    Fragment* _parent;

    // The attribute data type.
    RtToken _type;

    // The attribute variable name used in codegen.
    RtToken _variable;
};


/// @class Input
/// An input attribute.
class Input : public Attribute
{
  public:
    /// Constructor.
    Input(const RtToken& name, const RtToken& type) :
        Attribute(name, type),
        _connection(nullptr)
    {};

    /// Return the input value.
    const RtValue& getValue() const
    {
        return _value;
    }

    /// Return the input value.
    RtValue& getValue()
    {
        return _value;
    }

    /// Return the input connection, or nullptr
    /// if no connection exists on the input.
    Output* getConnection() const
    {
        return _connection;
    }

    /// Return true if this input is connected.
    bool isConnected() const
    {
        return _connection != nullptr;
    }

  protected:
    // The input value.
    RtValue _value;

    // Connection to the input.
    Output* _connection;

    friend class FragmentGraph;
};

/// A set of inputs connected downstream from an output.
using ConnectionSet = std::set<Input*>;

/// @class Output
/// An output attribute.
class Output : public Attribute
{
  public:
    /// Constructor.
    Output(const RtToken& name, const RtToken& type) : 
        Attribute(name, type)
    {};

    /// Return the set of connections to downstream inputs.
    const ConnectionSet& getConnections() const
    {
        return _connections;
    }

    /// Return true if this output is connected.
    bool isConnected() const
    {
        return !_connections.empty();
    }

  protected:
    // Connections from the output.
    ConnectionSet _connections;

    friend class FragmentGraph;
};

/// A shared pointer to an input attribute.
using InputPtr = RtSharedPtr<Input>;

/// A shared pointer to an output attribute.
using OutputPtr = RtSharedPtr<Output>;

/// A container of attributes.
using AttributeList = ObjectList<Attribute>;

/// A container of input attributes.
using InputList = ObjectList<Input>;

/// A container of output attributes.
using OutputList = ObjectList<Output>;

} // namespace Codegen
} // namespace MaterialX

#endif
