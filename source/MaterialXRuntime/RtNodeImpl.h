//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTNODEIMPL_H
#define MATERIALX_RTNODEIMPL_H

/// @file RtNodeImpl.h
/// TODO: Docs

#include <MaterialXRuntime/RtSchema.h>

namespace MaterialX
{

/// @class RtNodeImpl
/// Base class for schemas handling node implementation prims.
class RtNodeImpl : public RtTypedSchema
{
    DECLARE_TYPED_SCHEMA(RtNodeImpl)

public:
    /// Constructor.
    RtNodeImpl(const RtPrim& prim) : RtTypedSchema(prim) {}

    /// Set an identifier for the target used by this implementation.
    void setTarget(const RtToken& target);

    /// Return an identifier for the target used by this implementation.
    /// If an empty token is returned, this prim supports all targets.
    const RtToken& getTarget() const;

    /// Set the name of the nodedef this implementation applies to.
    void setNodeDef(const RtToken& nodedef);

    /// Return the name of the nodedef this implementation applies to.
    const RtToken& getNodeDef() const;

    /// Set an alternative name for this node for this specific target.
    /// This allows one to say that for this particular target, the native
    /// node or shader is called something else but is functionally equivalent
    /// to the node described by the nodedef.
    void setImplName(const RtToken& implname);

    /// Return an alternative name for this node and this specific target.
    /// This allows one to say that for this particular target, the native
    /// node or shader is called something else but is functionally equivalent
    /// to the node described by the nodedef.
    const RtToken& getImplName() const;

    /// Set a file containing the source code to use for this implementation.
    void setFile(const string& file);

    /// Return a file containing the source code used by this implementation.
    const string& getFile() const;

    /// Set a string containing the source code to use for this implementation.
    void setSourceCode(const string& source);

    /// Return a string containing the source code used by this implementation.
    const string& getSourceCode() const;

    /// Set the format used by the source code in this implementation.
    void setFormat(const RtToken& format);

    /// Return the format used by the source code in this implementation.
    const RtToken& getFormat() const;

    /// Set the function name to use for this implementation.
    void setFunction(const RtToken& function);

    /// Return the function name to use for this implementation.
    const RtToken& getFunction() const;
};

}

#endif
