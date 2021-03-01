//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTRELATIONSHIP_H
#define MATERIALX_RTRELATIONSHIP_H

/// @file
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtObject.h>

namespace MaterialX
{

class RtConnectionIterator;

/// @class RtRelationship
/// Object holding a relationship on a prim.
class RtRelationship : public RtObject
{
    RT_DECLARE_RUNTIME_OBJECT(RtRelationship)

public:
    /// Empty constructor.
    /// Creating an invalid object.
    RtRelationship() {}

    /// Construct from a data handle.
    RtRelationship(PvtDataHandle hnd);

    /// Return the name of this relationship.
    const RtToken& getName() const;

    /// Return true if this relationship has any targets.
    bool hasTargets() const;

    /// Return the number of targets
    size_t targetCount() const;

    /// Add a target to this relationship.
    void addTarget(const RtObject& target);

    /// Remove a target from this relationship.
    void removeTarget(const RtObject& target);

    /// Clear all targets from this relationship.
    void clearTargets();

    /// Set a target by index. No range check is performed and an entry
    /// for the given index must exists.
    void setTarget(const RtObject& obj, size_t index = 0);

    /// Return a target by index. No range check is performed and an entry
    /// for the given index must exists.
    RtObject getTarget(size_t index = 0) const;

    /// Return an iterator over all targets for this relationship.
    RtConnectionIterator getTargets() const;

    /// Return targets as a string
    string getTargetsAsString(const string& sep = ",") const;
};

}

#endif
