//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_OGSFXSYNTAX_H
#define MATERIALX_OGSFXSYNTAX_H

/// @file
/// OGSFX syntax class

#include <MaterialXGenGlsl/GlslSyntax.h>

namespace MaterialX
{

/// Syntax class for OgsFx
class GLESSyntax : public GlslSyntax
{
    using ParentClass = GLESSyntax;

public:
    GLESSyntax();

    static SyntaxPtr create() { return std::make_shared<GLESSyntax>(); }
};

} // namespace MaterialX

#endif
