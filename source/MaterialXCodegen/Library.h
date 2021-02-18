//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_LIBRARY_H
#define MATERIALX_CODEGEN_LIBRARY_H

/// @file
/// Library-wide includes and types.  This file should be the first include for
/// any public header in the MaterialXCodegen library.

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>

namespace MaterialX
{
namespace Codegen
{

class Syntax;
class TypeSyntax;
class Options;
class Context;
class FragmentGenerator;
class FragmentCompiler;
class Fragment;
class FragmentGraph;
class SourceFragment;

using SyntaxPtr = RtSharedPtr<Syntax>;
using TypeSyntaxPtr = RtSharedPtr<TypeSyntax>;
using OptionsPtr = RtSharedPtr<Options>;
using ContextPtr = RtSharedPtr<Context>;
using FragmentGeneratorPtr = RtSharedPtr<FragmentGenerator>;
using FragmentCompilerPtr = RtSharedPtr<FragmentCompiler>;
using FragmentPtr = RtSharedPtr<Fragment>;

/// @class NamedObject
/// Base class for named objects.
class NamedObject
{
  public:
    /// Constructor.
    NamedObject(const RtToken& name) : _name(name) {}

    /// Return the object name.
    const RtToken& getName() const
    {
        return _name;
    }

  protected:
    // The object name.
    RtToken _name;
};

/// @class ObjectList
/// A container of objects of templated class.
/// The templated class must inherit from NamedObject.
/// Handles both ordered access and access by name.
template<class T>
class ObjectList
{
public:
    using ObjectPtr = RtSharedPtr<T>;

    size_t size() const
    {
        return _objects.size();
    }

    size_t index(const RtToken& name) const
    {
        auto it = _indexByName.find(name);
        return it != _indexByName.end() ? it->second : size_t(-1);
    }

    T* get(const RtToken& name) const
    {
        return get(index(name));
    }

    T* get(size_t i) const
    {
        return i < _objects.size() ? _objects[i].get() : nullptr;
    }

    void add(const ObjectPtr& obj)
    {
        _objects.push_back(obj);
        _indexByName[obj->getName()] = _objects.size() - 1;
    }

    ObjectPtr remove(const RtToken& name)
    {
        auto it = _indexByName.find(name);
        if (it != _indexByName.end())
        {
            ObjectPtr obj = _objects[it->second];
            _objects.erase(_objects.begin() + it->second);
            _indexByName.erase(it);
            return obj;
        }
        return nullptr;
    }

    void clear()
    {
        _objects.clear();
        _indexByName.clear();
    }

private:
    vector<ObjectPtr> _objects;
    RtTokenMap<size_t> _indexByName;
};

} // namespace Codegen
} // namespace MaterialX

#endif
