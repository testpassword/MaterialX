//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/Impl/RtSubGraphImpl.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

namespace
{
    static const RtToken DEFAULT_PRIM_NAME("subgraphimpl1");
}

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

    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_PRIM_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtSubGraphImpl::initialize(const RtPrim& nodegraph)
{
}

RtPrim RtSubGraphImpl::getNodeGraph() const
{
    return RtPrim();
}

void RtSubGraphImpl::emitFunctionDefinition(const RtNode& node, GenContext& context, ShaderStage& stage) const
{

}

void RtSubGraphImpl::emitFunctionCall(const RtNode& node, GenContext& context, ShaderStage& stage) const
{

}

}
