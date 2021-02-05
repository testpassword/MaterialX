//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/RtValue.h>
#include <MaterialXRuntime/RtTypeDef.h>
#include <MaterialXRuntime/RtPrim.h>

#include <MaterialXRuntime/Private/PvtPrim.h>

#include <sstream>

namespace MaterialX
{

RtAllocator::~RtAllocator()
{
    free();
}

uint8_t* RtAllocator::alloc(size_t size)
{
    uint8_t* ptr = new uint8_t[size];
    _storage.push_back(ptr);
    return ptr;
}

void RtAllocator::free()
{
    for (uint8_t* ptr : _storage)
    {
        delete[] ptr;
    }
    _storage.clear();
}


RtValue::RtValue(const Matrix33& v, RtAllocator& allocator)
{
    // Allocate storage for the value.
    *_reinterpret_cast<Matrix33**>(&_data) = allocator.allocType<Matrix33>();

    // Copy the value.
    asMatrix33() = v;
}

RtValue::RtValue(const Matrix44& v, RtAllocator& allocator)
{
    // Allocate storage for the value.
    *_reinterpret_cast<Matrix44**>(&_data) = allocator.allocType<Matrix44>();

    // Copy the value.
    asMatrix44() = v;
}

RtValue::RtValue(const string& v, RtAllocator& allocator)
{
    // Allocate storage for the value.
    *_reinterpret_cast<string**>(&_data) = allocator.allocType<string>();

    // Copy the value.
    asString() = v;
}

RtValue::RtValue(const Matrix33& v, RtPrim owner) : RtValue(v, PvtObject::ptr<PvtPrim>(owner)->getAllocator()) {}
RtValue::RtValue(const Matrix44& v, RtPrim owner) : RtValue(v, PvtObject::ptr<PvtPrim>(owner)->getAllocator()) {}
RtValue::RtValue(const string& v, RtPrim owner) : RtValue(v, PvtObject::ptr<PvtPrim>(owner)->getAllocator()) {}

RtValue RtValue::createNew(const RtToken& type, RtAllocator& allocator)
{
    const RtTypeDef* typeDef = RtTypeDef::findType(type);
    if (!typeDef)
    {
        throw ExceptionRuntimeError("Type '" + type.str() + "' is not a registered type");
    }
    return typeDef->createValue(allocator);
}

RtValue RtValue::createNew(const RtToken& type, RtPrim owner)
{
    return createNew(type, PvtObject::ptr<PvtPrim>(owner)->getAllocator());
}

RtValue RtValue::clone(const RtToken& type, const RtValue& value, RtAllocator& allocator)
{
    RtValue clonedValue = createNew(type, allocator);
    copy(type, value, clonedValue);
    return clonedValue;
}

RtValue RtValue::clone(const RtToken& type, const RtValue& value, RtPrim owner)
{
    return clone(type, value, PvtObject::ptr<PvtPrim>(owner)->getAllocator());
}

void RtValue::copy(const RtToken& type, const RtValue& src, RtValue& dest)
{
    const RtTypeDef* typeDef = RtTypeDef::findType(type);
    if (!typeDef)
    {
        throw ExceptionRuntimeError("Type '" + type.str() + "' is not a registered type");
    }
    typeDef->copyValue(src, dest);
}

bool RtValue::compare(const RtToken& type, const RtValue& a, const RtValue& b)
{
    const RtTypeDef* typeDef = RtTypeDef::findType(type);
    if (!typeDef)
    {
        throw ExceptionRuntimeError("Type '" + type.str() + "' is not a registered type");
    }
    return typeDef->compareValue(a, b);
}

void RtValue::toString(const RtToken& type, const RtValue& src, string& dest)
{
    const RtTypeDef* typeDef = RtTypeDef::findType(type);
    if (!typeDef)
    {
        throw ExceptionRuntimeError("Type '" + type.str() + "' is not a registered type");
    }
    typeDef->toStringValue(src, dest);
}

void RtValue::fromString(const RtToken& type, const string& src, RtValue& dest)
{
    const RtTypeDef* typeDef = RtTypeDef::findType(type);
    if (!typeDef)
    {
        throw ExceptionRuntimeError("Type '" + type.str() + "' is not a registered type");
    }
    typeDef->fromStringValue(src, dest);
}

}
