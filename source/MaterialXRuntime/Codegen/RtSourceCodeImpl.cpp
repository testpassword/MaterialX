//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtSourceCodeImpl.h>

#include <MaterialXRuntime/Private/Codegen/PvtSourceCodeImpl.h>
#include <MaterialXRuntime/Private/PvtPath.h>
#include <MaterialXRuntime/Private/PvtPrim.h>


namespace MaterialX
{

DEFINE_TYPED_SCHEMA(RtSourceCodeImpl, "nodeimpl:sourcecodeimpl");

RtPrim RtSourceCodeImpl::createPrim(const RtToken& typeName, const RtToken& name, RtPrim parent)
{
    PvtPrim::validateCreation(_typeInfo, typeName, name, parent.getPath());

    static const RtToken DEFAULT_NAME("sourcecodeimpl1");
    const RtToken primName = name == EMPTY_TOKEN ? DEFAULT_NAME : name;
    PvtDataHandle primH = PvtPrim::createNew<PvtSourceCodeImpl>(&_typeInfo, primName, PvtObject::ptr<PvtPrim>(parent));

    return primH;
}

void RtSourceCodeImpl::setFile(const string& file)
{
    prim()->asA<PvtSourceCodeImpl>()->setFile(file);
}

const string& RtSourceCodeImpl::getFile() const
{
    return prim()->asA<PvtSourceCodeImpl>()->getFile();
}

void RtSourceCodeImpl::setSourceCode(const string& source)
{
    prim()->asA<PvtSourceCodeImpl>()->setSourceCode(source);
}

const string& RtSourceCodeImpl::getSourceCode() const
{
    return prim()->asA<PvtSourceCodeImpl>()->getSourceCode();
}

void RtSourceCodeImpl::setFormat(const RtToken& format)
{
    prim()->asA<PvtSourceCodeImpl>()->setFormat(format);
}

const RtToken& RtSourceCodeImpl::getFormat() const
{
    return prim()->asA<PvtSourceCodeImpl>()->getFormat();
}

void RtSourceCodeImpl::setFunction(const string& function)
{
    prim()->asA<PvtSourceCodeImpl>()->setFunction(function);
}

const string& RtSourceCodeImpl::getFunction() const
{
    return prim()->asA<PvtSourceCodeImpl>()->getFunction();
}

}
