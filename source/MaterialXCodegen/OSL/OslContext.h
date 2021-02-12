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
/// Class for context data needed by codegen to OSL.
class OslContext : public Context
{
  public:
    /// Identifier for this target.
    static const RtToken TARGET;

  public:
    /// Constructor.
    OslContext(OptionsPtr options);

    /// Create a new OSL context.
    static ContextPtr create(OptionsPtr options);

    /// Return the target identifier.
    const RtToken& getTarget() const override;

    /// Return the syntax object for this target.
    const Syntax& getSyntax() const override;

    /// Create a fragment generator for this target.
    FragmentGeneratorPtr createGenerator() const override;

    /// Create a fragment compiler for this target.
    FragmentCompilerPtr createCompiler() const override;

  protected:
    SyntaxPtr _syntax;
};

} // namespace Codegen
} // namespace MaterialX


#endif
