//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#include <MaterialXGenCross/HLSLShaderGenerator.h>
namespace MaterialX
{
    HLSLShaderGenerator::HLSLShaderGenerator() :
        ShaderGenerator(HLSLSyntax::create())
    {}

    ShaderPtr HLSLShaderGenerator::generate(const string& name, ElementPtr element, 
                                                                GenContext& context) const
    {
        name;
        element;
        context;
        return nullptr;
    }
}
