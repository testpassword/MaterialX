//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTGEOMPROPDEF_H
#define MATERIALX_RTGEOMPROPDEF_H

/// @file RtGeomPropDef.h
/// TODO: Docs

#include <MaterialXRuntime/RtSchema.h>

namespace MaterialX
{

/// @class RtGeomPropDef
/// Schemas for geompropdef prims, handling the definition of a primvar.
class RtGeomPropDef : public RtTypedSchema
{
    DECLARE_TYPED_SCHEMA(RtNodeImpl)

public:
    /// Constructor.
    RtGeomPropDef(const RtPrim& prim) : RtTypedSchema(prim) {}

    /// Set the geomprop name.
    void setGeomProp(const RtToken& name);

    /// Return the geomprop name.
    const RtToken& getGeomProp() const;

    /// Set the geomprop type.
    void setType(const RtToken& type);

    /// Return the geomprop type.
    const RtToken& getType() const;

    /// Set the geomprop coordinate space.
    void setSpace(const RtToken& space);

    /// Return the geomprop coordinate space.
    const RtToken& getSpace() const;

    /// Set the geomprop texture coordinates index.
    void setIndex(int index);

    /// Return the geomprop texture coordinates index.
    int getIndex() const;
};

}

#endif
