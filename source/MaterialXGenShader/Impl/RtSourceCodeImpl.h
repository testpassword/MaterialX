//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTSOURCECODEIMPL_H
#define MATERIALX_RTSOURCECODEIMPL_H

/// @file RtSourceCodeImpl.h
/// TODO: Docs

#include <MaterialXGenShader/Impl/RtCodegenImpl.h>

namespace MaterialX
{

/// @class RtSourceCodeImpl
/// Node implementation using shader source code.
class RtSourceCodeImpl : public RtCodegenImpl
{
    DECLARE_TYPED_SCHEMA(RtSourceCodeImpl)

public:
    /// Constructor.
    RtSourceCodeImpl(const RtPrim& prim) : RtCodegenImpl(prim) {}

    /// Emit function definition for the given node instance in the given context.
    void emitFunctionDefinition(const RtNode& node, GenContext& context, ShaderStage& stage) const override;

    /// Emit the function call or inline source code for given node instance in the given context.
    void emitFunctionCall(const RtNode& node, GenContext& context, ShaderStage& stage) const override;

    /// Set a string containing the source code used to use for this implementation.
    void setSourceCode(const string& source) const;

    /// Return a string containing the source code used by this implementation.
    const string& getSourceCode() const;

    /// Set the format used by the source code in this implementation.
    void setFormat(const RtToken& format);

    /// Return the format used by the source code in this implementation.
    const RtToken& getFormat() const;
};

}

#endif
