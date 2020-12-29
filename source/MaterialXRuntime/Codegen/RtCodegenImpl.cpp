//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtCodegenImpl.h>

#include <MaterialXRuntime/Private/Codegen/PvtCodegenImpl.h>

namespace MaterialX
{

void RtCodegenImpl::addInputs(const RtNode& /*node*/, RtCodegenContext& /*context*/, RtFragment& /*frag*/) const
{
}

void RtCodegenImpl::createVariables(const RtNode& /*node*/, RtCodegenContext& /*context*/, RtFragment& /*frag*/) const
{
}

void RtCodegenImpl::emitFunctionDefinition(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const
{
    prim()->asA<PvtCodegenImpl>()->emitFunctionDefinition(node, context, frag);
}

void RtCodegenImpl::emitFunctionCall(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const
{
    prim()->asA<PvtCodegenImpl>()->emitFunctionCall(node, context, frag);
}

}
