//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtSubGraphImpl.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

DEFINE_TYPED_SCHEMA(RtSubGraphImpl, "nodeimpl:subgraphimpl");

RtPrim RtSubGraphImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    if (typeName != _typeInfo.getShortTypeName())
    {
        throw ExceptionRuntimeError("Type names mismatch when creating prim '" + name.str() + "'");
    }
    if (!parent.getPath().isRoot())
    {
        throw ExceptionRuntimeError("A compoundimpl prim can only be created at the top / root level");
    }

    static const RtToken DEFAULT_NAME("subgraphimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    PvtPrim* prim = primH->asA<PvtPrim>();
    prim->createRelationship(Tokens::NODEIMPL);

    return primH;
}

void RtSubGraphImpl::initialize(const RtPrim& /*nodegraph*/)
{
}

RtPrim RtSubGraphImpl::getNodeGraph() const
{
    return RtPrim();
}

void RtSubGraphImpl::emitFunctionDefinition(const RtNode& /*node*/, GenContext& /*context*/, ShaderStage& /*stage*/) const
{
}

void RtSubGraphImpl::emitFunctionCall(const RtNode& /*node*/, GenContext& /*context*/, ShaderStage& /*stage*/) const
{
}

}
