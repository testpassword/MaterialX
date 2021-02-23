//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_BSDFS_H
#define MATERIALX_CODEGEN_BSDFS_H

/// @file Bsdfs.h
/// Bsdf fragments.

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

/// @class SheenBsdfFragment
/// A fragment for the sheen bsdf node.
class SheenBsdfFragment : public SourceFragment
{
    DECLARE_FRAGMENT_CLASS(SheenBsdfFragment)

public:
    void finalize(const Context& context) override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
