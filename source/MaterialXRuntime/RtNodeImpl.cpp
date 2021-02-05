//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtNodeImpl.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

DEFINE_TYPED_SCHEMA(RtNodeImpl, "nodeimpl");

RtPrim RtNodeImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("nodeimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtNodeImpl::setTarget(const RtToken& target)
{
    RtTypedValue* data = addMetadata(Tokens::TARGET, RtType::TOKEN);
    data->getValue().asToken() = target;
}

const RtToken& RtNodeImpl::getTarget() const
{
    const RtTypedValue* data = getMetadata(Tokens::TARGET, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setNodeDef(const RtToken& language)
{
    RtTypedValue* data = addMetadata(Tokens::NODEDEF, RtType::TOKEN);
    data->getValue().asToken() = language;
}

const RtToken& RtNodeImpl::getNodeDef() const
{
    const RtTypedValue* data = getMetadata(Tokens::NODEDEF, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setImplName(const RtToken& implname)
{
    RtTypedValue* data = addMetadata(Tokens::IMPLNAME, RtType::TOKEN);
    data->getValue().asToken() = implname;
}

const RtToken& RtNodeImpl::getImplName() const
{
    const RtTypedValue* data = getMetadata(Tokens::IMPLNAME, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setFile(const string& file)
{
    RtTypedValue* data = addMetadata(Tokens::FILE, RtType::STRING);
    data->getValue().asString() = file;
}

const string& RtNodeImpl::getFile() const
{
    const RtTypedValue* data = getMetadata(Tokens::FILE, RtType::STRING);
    return data ? data->getValue().asString() : EMPTY_STRING;
}

void RtNodeImpl::setSourceCode(const string& source)
{
    RtTypedValue* data = addMetadata(Tokens::SOURCECODE, RtType::STRING);
    data->getValue().asString() = source;
}

const string& RtNodeImpl::getSourceCode() const
{
    const RtTypedValue* data = getMetadata(Tokens::SOURCECODE, RtType::STRING);
    return data ? data->getValue().asString() : EMPTY_STRING;
}

void RtNodeImpl::setFormat(const RtToken& format)
{
    RtTypedValue* data = addMetadata(Tokens::FORMAT, RtType::TOKEN);
    data->getValue().asToken() = format;
}

const RtToken& RtNodeImpl::getFormat() const
{
    const RtTypedValue* data = getMetadata(Tokens::FORMAT, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setFunction(const RtToken& function)
{
    RtTypedValue* data = addMetadata(Tokens::FUNCTION, RtType::TOKEN);
    data->getValue().asToken() = function;
}

const RtToken& RtNodeImpl::getFunction() const
{
    const RtTypedValue* data = getMetadata(Tokens::FUNCTION, RtType::TOKEN);
    return data ? data->getValue().asToken() : EMPTY_TOKEN;
}

}
