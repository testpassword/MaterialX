//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CONVERT_H
#define MATERIALX_CODEGEN_CONVERT_H

/// @file Convert.h
/// Convert fragment.

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

/// @class ConvertFragment
/// A fragment for type conversions.
class ConvertFragment : public Fragment
{
public:
    /// Constructor.
    ConvertFragment(const RtToken& name);

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

    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

/// @class SwizzleFragment
/// A fragment for value swizzling.
class SwizzleFragment : public Fragment
{
public:
    /// Constructor.
    SwizzleFragment(const RtToken& name);

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

    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
