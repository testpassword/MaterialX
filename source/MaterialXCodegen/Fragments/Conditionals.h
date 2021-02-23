//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
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

    /// Generate the fragment source code.
    void emitFunctionCall(const Context& context, SourceCode& result) const override;

  protected:
    /// Private constructor.
    IfFragment(const RtToken& name);
};

/// @class IfEqualFragment
/// A fragment representing an if-equal condition.
class IfEqualFragment : public IfFragment
{
    DECLARE_FRAGMENT_CLASS(IfEqualFragment)

public:
    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};

/// @class IfGreaterFragment
/// A fragment representing an if-greater condition.
class IfGreaterFragment : public IfFragment
{
    DECLARE_FRAGMENT_CLASS(IfGreaterFragment)

public:
    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};

/// @class IfGreaterEqFragment
/// A fragment representing an if-greater-or-equal condition.
class IfGreaterEqFragment : public IfFragment
{
    DECLARE_FRAGMENT_CLASS(IfGreaterEqFragment)

public:
    /// Return the equality operator to use.
    const string& equalityOperator() const override;
};


/// @class SwitchFragment
/// A fragment representing a switch condition.
class SwitchFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(SwitchFragment)

public:
    /// Generate the fragment source code.
    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
