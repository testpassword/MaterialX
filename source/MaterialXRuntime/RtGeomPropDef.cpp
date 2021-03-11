//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtGeomPropDef.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>

namespace MaterialX
{

namespace
{
    // TODO: We should derive this from a data driven XML schema.
    class PvtGeomPropDefPrimSpec : public PvtPrimSpec
    {
    public:
        PvtGeomPropDefPrimSpec()
        {
            addPrimAttribute(Tokens::GEOMPROP, RtType::TOKEN);
            addPrimAttribute(Tokens::TYPE, RtType::TOKEN);
            addPrimAttribute(Tokens::SPACE, RtType::TOKEN);
            addPrimAttribute(Tokens::INDEX, RtType::INTEGER);
        }
    };
}

DEFINE_TYPED_SCHEMA(RtGeomPropDef, "geompropdef");

RtPrim RtGeomPropDef::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("geompropdef1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

const RtPrimSpec& RtGeomPropDef::getPrimSpec() const
{
    static const PvtGeomPropDefPrimSpec s_primSpec;
    return s_primSpec;
}

void RtGeomPropDef::setGeomProp(const RtToken& geomprop)
{
    RtTypedValue* attr = createAttribute(Tokens::GEOMPROP, RtType::TOKEN);
    attr->getValue().asToken() = geomprop;
}

const RtToken& RtGeomPropDef::getGeomProp() const
{
    const RtTypedValue* attr = getAttribute(Tokens::GEOMPROP, RtType::TOKEN);
    return attr->asToken();
}

void RtGeomPropDef::setType(const RtToken& type)
{
    RtTypedValue* attr = createAttribute(Tokens::TYPE, RtType::TOKEN);
    attr->getValue().asToken() = type;
}

const RtToken& RtGeomPropDef::getType() const
{
    const RtTypedValue* attr = getAttribute(Tokens::TYPE, RtType::TOKEN);
    return attr->asToken();
}

void RtGeomPropDef::setSpace(const RtToken& space)
{
    RtTypedValue* attr = createAttribute(Tokens::SPACE, RtType::TOKEN);
    attr->getValue().asToken() = space;
}

const RtToken& RtGeomPropDef::getSpace() const
{
    const RtTypedValue* attr = getAttribute(Tokens::SPACE, RtType::TOKEN);
    return attr->asToken();
}

void RtGeomPropDef::setIndex(int index)
{
    RtTypedValue* attr = createAttribute(Tokens::INDEX, RtType::INTEGER);
    attr->getValue().asInt() = index;
}

int RtGeomPropDef::getIndex() const
{
    const RtTypedValue* attr = getAttribute(Tokens::INDEX, RtType::INTEGER);
    return attr->asInt();
}

}
