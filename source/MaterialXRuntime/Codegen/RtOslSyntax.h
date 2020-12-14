//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_RTOSLSYNTAX_H
#define MATERIALX_RTOSLSYNTAX_H

/// @file
/// OSL syntax class

#include <MaterialXGenShader/Syntax.h>

namespace MaterialX
{

/// @class RtOslSyntax
/// Syntax class for OSL (Open Shading Language)
class RtOslSyntax : public Syntax
{
public:
    RtOslSyntax();

    static SyntaxPtr create() { return std::make_shared<RtOslSyntax>(); }

    const string& getOutputQualifier() const override;
    const string& getConstantQualifier() const override { return EMPTY_STRING; };
    const string& getSourceFileExtension() const override { return SOURCE_FILE_EXTENSION; };

    static const string OUTPUT_QUALIFIER;
    static const string SOURCE_FILE_EXTENSION;
    static const StringVec VECTOR_MEMBERS;
    static const StringVec VECTOR2_MEMBERS;
    static const StringVec VECTOR4_MEMBERS;
    static const StringVec COLOR2_MEMBERS;
    static const StringVec COLOR4_MEMBERS;
};

} // namespace MaterialX

#endif
