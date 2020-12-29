//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/Codegen/PvtCodegenImpl.h>

namespace MaterialX
{

PvtCodegenImpl::PvtCodegenImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent)
    : PvtPrim(typeInfo, name, parent)
{
}

}
