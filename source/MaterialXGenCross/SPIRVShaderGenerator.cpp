//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//
#include <MaterialXGenCross/SPIRVShaderGenerator.h>
namespace MaterialX
{
    SPIRVShaderGenerator::SPIRVShaderGenerator() :
        GlslShaderGenerator()
    {}

    ShaderPtr SPIRVShaderGenerator::generate(const string& name, ElementPtr element,
                                                                 GenContext& context) const
    {
        return GlslShaderGenerator::generate(name, element, context);
    }

}
