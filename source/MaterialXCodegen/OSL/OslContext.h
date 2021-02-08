//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_OSLCONTEXT_H
#define MATERIALX_CODEGEN_OSLCONTEXT_H

/// @file OslContext.h
/// TODO: Docs

#include <MaterialXCodegen/Context.h>

namespace MaterialX
{
namespace Codegen
{

/// @class OslContext
/// Class for context data needed by OSL fragment generators.
class OslContext : public Context
{
public:
    /// Constructor.
    OslContext(FragmentGeneratorPtr generator, OptionsPtr options);
};

} // namepspace Codegen
} // namepspace MaterialX


#endif
