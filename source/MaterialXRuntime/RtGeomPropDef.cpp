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
    // Private implementation of geompropdef prims.
    class PvtGeomPropDefPrim : public PvtPrim
    {
    public:
        PvtGeomPropDefPrim(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent) : 
            PvtPrim(typeInfo, name, parent),
            type(RtType::VECTOR3),
            space(Tokens::OBJECT),
            index(0)
        {}

        RtToken geomprop;
        RtToken type;
        RtToken space;
        int index;
    };
}

DEFINE_TYPED_SCHEMA(RtGeomPropDef, "geompropdef");

RtPrim RtGeomPropDef::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("geompropdef1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew<PvtGeomPropDefPrim>(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtGeomPropDef::setGeomProp(const RtToken& geomprop)
{
    prim()->asA<PvtGeomPropDefPrim>()->geomprop = geomprop;
}

const RtToken& RtGeomPropDef::getGeomProp() const
{
    return prim()->asA<PvtGeomPropDefPrim>()->geomprop;
}

void RtGeomPropDef::setType(const RtToken& type)
{
    prim()->asA<PvtGeomPropDefPrim>()->type = type;
}

const RtToken& RtGeomPropDef::getType() const
{
    return prim()->asA<PvtGeomPropDefPrim>()->type;
}

void RtGeomPropDef::setSpace(const RtToken& space)
{
    prim()->asA<PvtGeomPropDefPrim>()->space = space;
}

const RtToken& RtGeomPropDef::getSpace() const
{
    return prim()->asA<PvtGeomPropDefPrim>()->space;
}

void RtGeomPropDef::setIndex(int index)
{
    prim()->asA<PvtGeomPropDefPrim>()->index = index;
}

int RtGeomPropDef::getIndex() const
{
    return prim()->asA<PvtGeomPropDefPrim>()->index;
}

}
