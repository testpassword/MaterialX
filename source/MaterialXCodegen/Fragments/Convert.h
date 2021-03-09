//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_CONVERT_H
#define MATERIALX_CODEGEN_CONVERT_H

/// @file Convert.h
/// Fragments for converting inputs to other data types
/// or manipulating the channels on multichannel types.

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
    DECLARE_FRAGMENT_CLASS(ConvertFragment)

public:
    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

/// @class SwizzleFragment
/// A fragment for value swizzling.
class SwizzleFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(SwizzleFragment)

public:
    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

/// @class CombineFragment
/// A fragment for combining multiple input streams
/// into a single multichannel output stream.
class CombineFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(CombineFragment)

public:
    void emitFunctionCall(const Context& context, SourceCode& result) const override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
