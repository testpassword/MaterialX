//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTGRAPHIMPL_H
#define MATERIALX_RTGRAPHIMPL_H

/// @file RtGraphImpl.h
/// TODO: Docs

#include <MaterialXRuntime/Codegen/RtCodegenImpl.h>
#include <MaterialXRuntime/RtNodeGraph.h>

namespace MaterialX
{

/// @class RtGraphImpl
/// A node implementation using a graph.
class RtGraphImpl : public RtCodegenImpl
{
    DECLARE_TYPED_SCHEMA(RtGraphImpl)

public:
    /// Constructor.
    RtGraphImpl(const RtPrim& prim) : RtCodegenImpl(prim) {}

    /// Initialize with the nodegraph to use as implementation.
    void initialize(const RtNodeGraph& nodegraph);

    /// Emit function definition for the given node instance in the given context.
    void emitFunctionDefinition(const RtNode& node, GenContext& context, ShaderStage& stage) const override;

    /// Emit the function call for the given node instance in the given context.
    void emitFunctionCall(const RtNode& node, GenContext& context, ShaderStage& stage) const override;

private:
    friend class PvtGraphImpl;
};

}

#endif
