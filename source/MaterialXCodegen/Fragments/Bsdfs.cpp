//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragments/Bsdfs.h>

#include <MaterialXRuntime/Tokens.h>

namespace MaterialX
{
namespace Codegen
{

DEFINE_FRAGMENT_CLASS(SheenBsdfFragment, SourceFragment)

void SheenBsdfFragment::finalize(const Context&)
{
    createInput(Tokens::BASE, RtType::BSDF);
}

} // namespace Codegen
} // namespace MaterialX
