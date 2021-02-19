//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CONDITIONALS_H
#define MATERIALX_CODEGEN_CONDITIONALS_H

/// @file Conditionals.h
/// Fragments representing conditionals.

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

/// @class IfFragment
/// Abstract base class for fragments representing if-else.
class IfFragment : public Fragment
{
public:
    /// Return the equality operator to use.
    virtual const string& equalityOperator() const = 0;

    void emitFunctionCall(const Context& context, SourceCode& result) const override;

  protected:
    /// Private constructor.
      IfFragment(const RtToken& name);
};

/// @class IfEqualFragment
/// A fragments representing an if-equal conditions.
class IfEqualFragment : public IfFragment
{
public:
    /// Constructor.
    IfEqualFragment(const RtToken& name);

    /// Create a new instance of this class.
    static FragmentPtr create(const RtToken& name);

    /// Return the class name for this fragment.
    static const RtToken& className();

    /// Create a copy of this fragment.
    FragmentPtr clone() const override;

    /// Return the fragment class name.
    const RtToken& getClassName() const override
    {
        return className();
    }

    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};

/// @class IfGreaterFragment
/// A fragments representing an if-greater conditions.
class IfGreaterFragment : public IfFragment
{
public:
    /// Constructor.
    IfGreaterFragment(const RtToken& name);

    /// Create a new instance of this class.
    static FragmentPtr create(const RtToken& name);

    /// Return the class name for this fragment.
    static const RtToken& className();

    /// Create a copy of this fragment.
    FragmentPtr clone() const override;

    /// Return the fragment class name.
    const RtToken& getClassName() const override
    {
        return className();
    }

    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};

/// @class IfGreaterEqFragment
/// A fragments representing an if-greater-or-equal conditions.
class IfGreaterEqFragment : public IfFragment
{
public:
    /// Constructor.
    IfGreaterEqFragment(const RtToken& name);

    /// Create a new instance of this class.
    static FragmentPtr create(const RtToken& name);

    /// Return the class name for this fragment.
    static const RtToken& className();

    /// Create a copy of this fragment.
    FragmentPtr clone() const override;

    /// Return the fragment class name.
    const RtToken& getClassName() const override
    {
        return className();
    }

    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};


} // namespace Codegen
} // namespace MaterialX

#endif
