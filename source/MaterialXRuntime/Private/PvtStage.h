//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PVTSTAGE_H
#define MATERIALX_PVTSTAGE_H

#include <MaterialXRuntime/Private/PvtObject.h>
#include <MaterialXRuntime/Private/PvtPrim.h>
#include <MaterialXRuntime/Private/PvtPath.h>

#include <MaterialXRuntime/RtStage.h>

/// @file
/// TODO: Docs

namespace MaterialX
{

class PvtStage;

class PvtStageIterator
{
public:
    PvtStageIterator() :
        _current(nullptr)
    {
    }

    PvtStageIterator(PvtStage* stage, RtObjectPredicate predicate = nullptr) :
        _current(nullptr),
        _predicate(predicate)
    {
        _stack.push_back(std::make_tuple(stage, -1, -1));
        this->operator++();
    }

    bool operator==(const PvtStageIterator& other) const
    {
        return _current == other._current;
    }

    bool operator!=(const PvtStageIterator& other) const
    {
        return _current != other._current;
    }

    PvtStageIterator& operator++();

    const PvtDataHandle& operator*() const
    {
        return _current;
    }

    bool isDone() const
    {
        return _current != nullptr;
    }

    void abort()
    {
        _current = nullptr;
    }

    PvtStageIterator& begin()
    {
        return *this;
    }

    static const PvtStageIterator& end()
    {
        static const PvtStageIterator NULL_STAGE_ITERATOR;
        return NULL_STAGE_ITERATOR;
    }

private:
    using StackFrame = std::tuple<PvtStage*, int, int>;
    PvtDataHandle _current;
    RtObjectPredicate _predicate;
    vector<StackFrame> _stack;
};


using RtStageVec = vector<RtStagePtr>;
using RtStageSet = std::set<const RtStage*>;

class PvtStage
{
public:
    PvtStage(const RtToken& name, RtStageWeakPtr owner);

    static inline PvtStage* ptr(const RtStagePtr& s)
    {
        return static_cast<PvtStage*>(s->_ptr);
    }

    const RtToken& getName() const
    {
        return _name;
    }

    PvtPrim* createPrim(const PvtPath& path, const RtToken& typeName);

    PvtPrim* createPrim(const PvtPath& parentPath, const RtToken& name, const RtToken& typeName);

    void removePrim(const PvtPath& path);

    void disposePrim(const PvtPath& path);

    void restorePrim(const PvtPath& parentPath, const RtPrim& prim);

    RtToken renamePrim(const PvtPath& path, const RtToken& newName);

    RtToken reparentPrim(const PvtPath& path, const PvtPath& newParentPath);

    PvtPrim* getPrimAtPath(const PvtPath& path);

    RtPrimIterator getPrims(RtObjectPredicate predicate = nullptr);

    PvtPrim* getRootPrim()
    {
        return _root->asA<PvtPrim>();
    }

    PvtPath getPath()  const
    {
        return _root->asA<PvtPrim>()->getPath();
    }

    const RtTokenVec& getSourceUri() const
    {
        return _sourceUri;
    }

    void addSourceUri(const RtToken& uri)
    {
        _sourceUri.push_back(uri);
    }

    void addReference(RtStagePtr stage);

    void removeReference(const RtToken& name);

    void removeReferences();

    RtStagePtr getReference(const RtToken& name) const;

    const RtStageVec& getAllReferences() const
    {
        return _refStages;
    }

    PvtStageIterator traverse(RtObjectPredicate predicate = nullptr)
    {
        return PvtStageIterator(this, predicate);
    }

protected:
    PvtPrim* getPrimAtPathLocal(const PvtPath& path);

    void setName(const RtToken& name)
    {
        _name = name;
    }

    class RootPrim : public PvtPrim
    {
    public:
        RootPrim(RtStageWeakPtr stage);

        RtStageWeakPtr getStage() const { return _stage; }

    protected:
        RtStageWeakPtr _stage;
        static const RtTypeInfo _typeInfo;
    };

    RtToken _name;
    PvtDataHandle _root;

    size_t _selfRefCount;
    RtStageVec _refStages;
    RtStageSet _refStagesSet;

    RtTokenVec _sourceUri;

    friend class RtStage;
    friend class PvtObject;
};

}

#endif