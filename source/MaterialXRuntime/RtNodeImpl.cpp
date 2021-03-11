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
namespace
{
    // TODO: We should derive this from a attr driven XML schema.
    class PvtNodeImplPrimSpec : public PvtPrimSpec
    {
    public:
        PvtNodeImplPrimSpec()
        {
            addPrimAttribute(Tokens::DOC, RtType::STRING);
            addPrimAttribute(Tokens::NODEDEF, RtType::TOKEN);
            addPrimAttribute(Tokens::TARGET, RtType::TOKEN);
            addPrimAttribute(Tokens::FILE, RtType::STRING);
            addPrimAttribute(Tokens::SOURCECODE, RtType::STRING);
            addPrimAttribute(Tokens::FUNCTION, RtType::STRING);
            addPrimAttribute(Tokens::FORMAT, RtType::TOKEN);
        }
    };
}

DEFINE_TYPED_SCHEMA(RtNodeImpl, "nodeimpl");

RtPrim RtNodeImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("nodeimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

const RtPrimSpec& RtNodeImpl::getPrimSpec() const
{
    static const PvtNodeImplPrimSpec s_primSpec;
    return s_primSpec;
}

void RtNodeImpl::setTarget(const RtToken& target)
{
    RtTypedValue* attr = createAttribute(Tokens::TARGET, RtType::TOKEN);
    attr->getValue().asToken() = target;
}

const RtToken& RtNodeImpl::getTarget() const
{
    const RtTypedValue* attr = getAttribute(Tokens::TARGET, RtType::TOKEN);
    return attr ? attr->asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setNodeDef(const RtToken& language)
{
    RtTypedValue* attr = createAttribute(Tokens::NODEDEF, RtType::TOKEN);
    attr->asToken() = language;
}

const RtToken& RtNodeImpl::getNodeDef() const
{
    const RtTypedValue* attr = getAttribute(Tokens::NODEDEF, RtType::TOKEN);
    return attr ? attr->asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setImplName(const RtToken& implname)
{
    RtTypedValue* attr = createAttribute(Tokens::IMPLNAME, RtType::TOKEN);
    attr->asToken() = implname;
}

const RtToken& RtNodeImpl::getImplName() const
{
    const RtTypedValue* attr = getAttribute(Tokens::IMPLNAME, RtType::TOKEN);
    return attr ? attr->asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setFile(const string& file)
{
    RtTypedValue* attr = createAttribute(Tokens::FILE, RtType::STRING);
    attr->asString() = file;
}

const string& RtNodeImpl::getFile() const
{
    const RtTypedValue* attr = getAttribute(Tokens::FILE, RtType::STRING);
    return attr ? attr->asString() : EMPTY_STRING;
}

void RtNodeImpl::setSourceCode(const string& source)
{
    RtTypedValue* attr = createAttribute(Tokens::SOURCECODE, RtType::STRING);
    attr->asString() = source;
}

const string& RtNodeImpl::getSourceCode() const
{
    const RtTypedValue* attr = getAttribute(Tokens::SOURCECODE, RtType::STRING);
    return attr ? attr->asString() : EMPTY_STRING;
}

void RtNodeImpl::setFormat(const RtToken& format)
{
    RtTypedValue* attr = createAttribute(Tokens::FORMAT, RtType::TOKEN);
    attr->asToken() = format;
}

const RtToken& RtNodeImpl::getFormat() const
{
    const RtTypedValue* attr = getAttribute(Tokens::FORMAT, RtType::TOKEN);
    return attr ? attr->asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setFunction(const RtToken& function)
{
    RtTypedValue* attr = createAttribute(Tokens::FUNCTION, RtType::TOKEN);
    attr->asToken() = function;
}

const RtToken& RtNodeImpl::getFunction() const
{
    const RtTypedValue* attr = getAttribute(Tokens::FUNCTION, RtType::TOKEN);
    return attr ? attr->asToken() : EMPTY_TOKEN;
}

void RtNodeImpl::setNodeGraph(RtPrim nodegraph)
{
    RtRelationship rel = getPrim().getRelationship(Tokens::NODEGRAPH);
    if (!rel)
    {
        rel = getPrim().createRelationship(Tokens::NODEGRAPH);
    }
    rel.connect(nodegraph);
}

RtPrim RtNodeImpl::getNodeGraph() const
{
    RtRelationship rel = getPrim().getRelationship(Tokens::NODEGRAPH);
    return (rel && rel.hasConnections() ? rel.getConnection() : RtPrim());
}

}
