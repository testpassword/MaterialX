//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtGraphImpl.h>

#include <MaterialXRuntime/Private/Codegen/PvtGraphImpl.h>

namespace MaterialX
{

DEFINE_TYPED_SCHEMA(RtGraphImpl, "nodeimpl:graphimpl");

RtPrim RtGraphImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("graphimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew<PvtGraphImpl>(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtGraphImpl::initialize(const RtNodeGraph& nodegraph)
{
    prim()->asA<PvtGraphImpl>()->initialize(nodegraph);
}

}
