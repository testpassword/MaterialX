//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_PVTCODEGENIMPL_H
#define MATERIALX_PVTCODEGENIMPL_H

#include <MaterialXRuntime/Private/PvtPrim.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/Codegen/RtGraphImpl.h>

namespace MaterialX
{

// Base class for all private node implementations that use shader code generation.
class PvtCodegenImpl : public PvtPrim
{
public:
    PvtCodegenImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent);

    virtual ~PvtCodegenImpl() {}

    virtual void emitFunctionDefinition(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const = 0;
    virtual void emitFunctionCall(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const = 0;
};

}

#endif
