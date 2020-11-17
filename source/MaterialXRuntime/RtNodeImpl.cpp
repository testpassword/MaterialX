//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeImpl.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

namespace
{
    static const RtToken METADATA_TARGET("target");
    static const RtToken METADATA_NODEDEF("nodedef");
    static const RtToken METADATA_IMPLNAME("implname");
    static const RtToken DEFAULT_PRIM_NAME("nodeimpl1");
}

DEFINE_TYPED_SCHEMA(RtNodeImpl, "nodeimpl");

RtPrim RtNodeImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    if (typeName != _typeInfo.getShortTypeName())
    {
        throw ExceptionRuntimeError("Type names mismatch when creating prim '" + name.str() + "'");
    }
    if (!parent.getPath().isRoot())
    {
        throw ExceptionRuntimeError("A nodeimpl prim can only be created at the top / root level");
    }

    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_PRIM_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtNodeImpl::setTarget(const RtToken& target)
{
    RtTypedValue* data = getMetadata(METADATA_TARGET, RtType::TOKEN);
    data->getValue().asToken() = target;
}

const RtToken& RtNodeImpl::getTarget() const
{
    const RtTypedValue* data = getMetadata(METADATA_TARGET, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setNodeDef(const RtToken& language)
{
    RtTypedValue* data = addMetadata(METADATA_NODEDEF, RtType::TOKEN);
    data->getValue().asToken() = language;
}

const RtToken& RtNodeImpl::getNodeDef() const
{
    const RtTypedValue* data = getMetadata(METADATA_NODEDEF, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setImplName(const RtToken& implname)
{
    RtTypedValue* data = getMetadata(METADATA_IMPLNAME, RtType::TOKEN);
    data->getValue().asToken() = implname;
}

const RtToken& RtNodeImpl::getImplName() const
{
    const RtTypedValue* data = getMetadata(METADATA_IMPLNAME, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

}
