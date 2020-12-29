//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTCODEGENIMPL_H
#define MATERIALX_RTCODEGENIMPL_H

/// @file RtCodegenImpl.h
/// TODO: Docs

#include <MaterialXRuntime/RtNodeImpl.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>

namespace MaterialX
{

/// @class RtCodegenImpl
/// Base class for all node implementations that use shader code generation.
class RtCodegenImpl : public RtNodeImpl
{
public:
    /// Constructor.
    RtCodegenImpl(const RtPrim& prim) : RtNodeImpl(prim) {}

    /// Add additional inputs on the node if needed by the implementation.
    void addInputs(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const;

    /// Create shader variables needed for the implementation of this node (e.g. uniforms, inputs and outputs).
    /// Used if the node requires explicit input data from the application.
    void createVariables(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const;

    /// Emit function definition for the given node instance in the given context.
    void emitFunctionDefinition(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const;

    /// Emit the function call or inline source code for given node instance in the given context.
    void emitFunctionCall(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const;
};

}

#endif
