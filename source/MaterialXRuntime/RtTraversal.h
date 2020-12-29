//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTTRAVERSAL_H
#define MATERIALX_RTTRAVERSAL_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtObject.h>
#include <MaterialXRuntime/RtAttribute.h>

namespace MaterialX
{

class PvtPrim;
class PvtOutput;
class RtPrim;
class RtStage;

/// Traversal predicate for specific object types.
template<typename T>
struct RtObjTypePredicate
{
    bool operator()(const RtObject& obj)
    {
        return obj.isA<T>();
    }
};

/// @class RtAttrIterator
/// Iterator for traversing over the attributes of a prim.
/// Using a predicate this iterator can be used to find all
/// attributes of a specific kind or type, etc.
class RtAttrIterator
{
public:
    /// Empty constructor.
    RtAttrIterator() :
        _prim(nullptr),
        _current(-1)
    {}

    /// Constructor, setting the prim to iterate on,
    /// and an optional predicate function.
    RtAttrIterator(const RtPrim& prim, RtObjectPredicate predicate = nullptr);

    /// Copy constructor.
    RtAttrIterator(const RtAttrIterator& other) :
        _prim(other._prim),
        _current(other._current),
        _predicate(other._predicate)
    {}

    /// Assignment operator.
    RtAttrIterator& operator=(const RtAttrIterator& other)
    {
        _prim = other._prim;
        _current = other._current;
        _predicate = other._predicate;
        return *this;
    }

    /// Equality operator.
    bool operator==(const RtAttrIterator& other) const
    {
        return _current == other._current &&
            _prim == other._prim;
    }

    /// Inequality operator.
    bool operator!=(const RtAttrIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current attribute.
    RtAttribute operator*() const;

    /// Iterate to the next sibling.
    RtAttrIterator& operator++();

    /// Return true if there are no more attribute in the iteration.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        *this = end();
    }

    /// Interpret this object as an iteration range,
    /// and return its begin iterator.
    RtAttrIterator& begin()
    {
        return *this;
    }

    /// Return the sentinel end iterator for this class.
    static const RtAttrIterator& end();

private:
    const PvtPrim* _prim;
    int _current;
    RtObjectPredicate _predicate;
};


/// @class RtPrimIterator
/// Iterator for traversing over the child prims (siblings) of a prim.
/// Using a predicate this iterator can be used to find all child prims
/// of a specific object type, or all child prims supporting a
/// specific API, etc.
class RtPrimIterator
{
public:
    /// Empty constructor.
    RtPrimIterator() :
        _prim(nullptr),
        _current(-1)
    {}

    /// Constructor, setting the prim to iterate on,
    /// and an optional predicate function.
    RtPrimIterator(const RtPrim& prim, RtObjectPredicate predicate = nullptr);

    /// Copy constructor.
    RtPrimIterator(const RtPrimIterator& other) :
        _prim(other._prim),
        _current(other._current),
        _predicate(other._predicate)
    {}

    /// Assignment operator.
    RtPrimIterator& operator=(const RtPrimIterator& other)
    {
        _prim = other._prim;
        _current = other._current;
        _predicate = other._predicate;
        return *this;
    }

    /// Equality operator.
    bool operator==(const RtPrimIterator& other) const
    {
        return _current == other._current &&
            _prim == other._prim;
    }

    /// Inequality operator.
    bool operator!=(const RtPrimIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current siblings.
    RtPrim operator*() const;

    /// Iterate to the next sibling.
    RtPrimIterator& operator++();

    /// Return true if there are no more siblings in the iteration.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        *this = end();
    }

    /// Interpret this object as an iteration range,
    /// and return its begin iterator.
    RtPrimIterator& begin()
    {
        return *this;
    }

    /// Return the sentinel end iterator for this class.
    static const RtPrimIterator& end();

private:
    const PvtPrim* _prim;
    int _current;
    RtObjectPredicate _predicate;
};


/// @class RtConnectionIterator
/// Iterator for traversing the connections on an output or relationship.
class RtConnectionIterator
{
public:
    /// Empty constructor.
    RtConnectionIterator() :
        _ptr(nullptr),
        _current(-1)
    {}

    /// Constructor, setting the output or relationship to iterate on.
    RtConnectionIterator(const RtObject& obj);

    /// Copy constructor.
    RtConnectionIterator(const RtConnectionIterator& other) :
        _ptr(other._ptr),
        _current(other._current)
    {}

    /// Assignment operator.
    RtConnectionIterator& operator=(const RtConnectionIterator& other)
    {
        _ptr = other._ptr;
        _current = other._current;
        return *this;
    }

    /// Equality operator.
    bool operator==(const RtConnectionIterator& other) const
    {
        return _current == other._current &&
            _ptr == other._ptr;
    }

    /// Inequality operator.
    bool operator!=(const RtConnectionIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current object.
    RtObject operator*() const;

    /// Iterate to the next sibling.
    RtConnectionIterator& operator++();

    /// Return true if there are no more attribute in the iteration.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        *this = end();
    }

    /// Interpret this object as an iteration range,
    /// and return its begin iterator.
    RtConnectionIterator& begin()
    {
        return *this;
    }

    /// Return the sentinel end iterator for this class.
    static const RtConnectionIterator& end();

private:
    void* _ptr;
    int _current;
};

/// @class RtRelationshipIterator
/// Iterator for traversing the relationships of a primitive.
class RtRelationshipIterator
{
public:
    /// Empty constructor.
    RtRelationshipIterator() :
        _ptr(nullptr),
        _current(-1)
    {}

    /// Constructor, setting the output or relationship to iterate on.
    RtRelationshipIterator(const RtObject& obj);

    /// Copy constructor.
    RtRelationshipIterator(const RtRelationshipIterator& other) :
        _ptr(other._ptr),
        _current(other._current)
    {}

    /// Assignment operator.
    RtRelationshipIterator& operator=(const RtRelationshipIterator& other)
    {
        _ptr = other._ptr;
        _current = other._current;
        return *this;
    }

    /// Equality operator.
    bool operator==(const RtRelationshipIterator& other) const
    {
        return _current == other._current &&
            _ptr == other._ptr;
    }

    /// Inequality operator.
    bool operator!=(const RtRelationshipIterator& other) const
    {
        return !(*this == other);
    }

    /// Dereference this iterator, returning the current object.
    RtObject operator*() const;

    /// Iterate to the next sibling.
    RtRelationshipIterator& operator++();

    /// Return true if there are no more attribute in the iteration.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort()
    {
        *this = end();
    }

    /// Interpret this object as an iteration range,
    /// and return its begin iterator.
    RtRelationshipIterator& begin()
    {
        return *this;
    }

    /// Return the sentinel end iterator for this class.
    static const RtRelationshipIterator& end();

private:
    void* _ptr;
    int _current;
};

/// @class RtStageIterator
/// API for iterating over prims in a stage, including referenced stages.
/// Only stage level prims are returned. Using a predicate this iterator can be
/// used to find all prims of a specific object type, or all
/// prims supporting a specific API, etc.
class RtStageIterator
{
public:
    /// Empty constructor.
    RtStageIterator();

    /// Constructor, setting the stage to iterate on and optionally
    /// a predicate restricting the set of returned objects.
    RtStageIterator(const RtStagePtr& stage, RtObjectPredicate predicate = nullptr);

    /// Copy constructor.
    RtStageIterator(const RtStageIterator& other);

    /// Assignment operator.
    RtStageIterator& operator=(const RtStageIterator& other);

    /// Destructor.
    ~RtStageIterator();

    /// Equality operator.
    bool operator==(const RtStageIterator& other) const;

    /// Inequality operator.
    bool operator!=(const RtStageIterator& other) const;

    /// Iterate to the next element in the traversal.
    RtStageIterator& operator++();

    /// Dereference this iterator, returning the current object
    /// in the traversal.
    RtPrim operator*() const;

    /// Return true if there are no more objects in the traversal.
    bool isDone() const;

    /// Force the iterator to terminate the traversal.
    void abort();

    /// Interpret this object as an iteration range,
    /// and return its begin iterator.
    RtStageIterator& begin()
    {
        return *this;
    }

    /// Return the sentinel end iterator for this class.
    static const RtStageIterator& end();

private:
    void* _ptr;
};


/// @class RtEdge
/// An edge returned during graph traversal.
class RtEdge
{
public:
    RtEdge(const RtOutput& up, const RtInput& down) :
        upstream(up),
        downstream(down)
    {}
    RtOutput upstream;
    RtInput downstream;
};

/// @class RtGraphIterator
/// Iterator class for traversing edges between nodes in a graph.
class RtGraphIterator
{
public:
    /// Empty constructor.
    RtGraphIterator();

    /// Constructor, setting the output to start traversal on.
    explicit RtGraphIterator(const RtOutput& output);

    /// Destructor.
    ~RtGraphIterator() {}

    /// Equality operator.
    bool operator==(const RtGraphIterator& rhs) const
    {
        return _upstream == rhs._upstream &&
            _downstream == rhs._downstream &&
            _stack == rhs._stack;
    }

    /// Inequality operator.
    bool operator!=(const RtGraphIterator& rhs) const
    {
        return !(*this == rhs);
    }

    /// Dereference this iterator, returning the current edge in the traversal.
    RtEdge operator*() const
    {
        return RtEdge(_upstream, _downstream);
    }

    /// Iterate to the next edge in the traversal.
    /// @throws ExceptionFoundCycle if a cycle is encountered.
    RtGraphIterator& operator++();

    /// Return a reference to this iterator to begin traversal
    RtGraphIterator& begin()
    {
        return *this;
    }

    /// Return the end iterator.
    static const RtGraphIterator& end();

private:
    void extendPathUpstream(const RtOutput& upstream, const RtInput& downstream);
    void returnPathDownstream(const RtOutput& upstream);

    RtOutput _upstream;
    RtInput _downstream;
    using StackFrame = std::pair<RtOutput, RtAttrIterator>;
    std::vector<StackFrame> _stack;
    std::set<const PvtOutput*> _path;
};

}

#endif
