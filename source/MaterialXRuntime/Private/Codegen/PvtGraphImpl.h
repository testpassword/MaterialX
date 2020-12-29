//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_PVTGRAPHIMPL_H
#define MATERIALX_PVTGRAPHIMPL_H

#include <MaterialXRuntime/Private/PvtPrim.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/Codegen/RtGraphImpl.h>

namespace MaterialX
{

// Private implementation of graphimpl prim.
class PvtGraphImpl : public PvtPrim
{
public:
    PvtGraphImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent);
    PvtGraphImpl(const RtToken& name);

    ~PvtGraphImpl();

    void initialize(const RtNode& node);
    void initialize(const RtNodeGraph& nodegraph, const RtToken& output);

    const vector<PvtPrim*>& getNodes() const
    {
        return _nodeOrder;
    }

    static PvtDataHandle createNew(const RtNode& node);
    static PvtDataHandle createNew(const RtNodeGraph& nodegraph, const RtToken& output = EMPTY_TOKEN);

private:
    void clear();
    void traverse(const RtOutput& output);
    void sort();

    RtTokenMap<PvtPrim*> _nodes;
    vector<PvtPrim*> _nodeOrder;
};

}

#endif
