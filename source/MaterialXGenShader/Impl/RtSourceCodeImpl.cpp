//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXGenShader/Impl/RtSourceCodeImpl.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

DEFINE_TYPED_SCHEMA(RtSourceCodeImpl, "nodeimpl:sourcecodeimpl");

RtPrim RtSourceCodeImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    if (typeName != _typeInfo.getShortTypeName())
    {
        throw ExceptionRuntimeError("Type names mismatch when creating prim '" + name.str() + "'");
    }
    if (!parent.getPath().isRoot())
    {
        throw ExceptionRuntimeError("A sourcecodeimpl prim can only be created at the top / root level");
    }

    static const RtToken DEFAULT_NAME("sourcecodeimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtSourceCodeImpl::emitFunctionDefinition(const RtNode& /*node*/, GenContext& /*context*/, ShaderStage& /*stage*/) const
{
}

void RtSourceCodeImpl::emitFunctionCall(const RtNode& /*node*/, GenContext& /*context*/, ShaderStage& /*stage*/) const
{
}

}
