//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>
#include <MaterialXTest/MaterialXGenCross/GenCross.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/XmlIo.h>
#include <MaterialXGenShader/GenContext.h>

namespace mx = MaterialX;

enum LanguageTarget { SPIRV, HLSL };

static void generateCrossCode(LanguageTarget languageTarget)
{
    const mx::FilePath testRootPath = mx::FilePath::getCurrentPath() / mx::FilePath("resources/Materials/TestSuite");
    const mx::FilePath testRootPath2 = mx::FilePath::getCurrentPath() / mx::FilePath("resources/Materials/Examples/StandardSurface");
    mx::FilePathVec testRootPaths;
    testRootPaths.push_back(testRootPath2);
    const mx::FilePath libSearchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    const mx::FileSearchPath srcSearchPath(libSearchPath.asString());
    bool writeShadersToDisk = false;

    
    const mx::FilePath logPath(languageTarget == LanguageTarget::HLSL ? "genhlsl_generate_test.txt" : "genspirv_generate_test.txt");
    const mx::GenOptions genOptions;
    mx::FilePath optionsFilePath = testRootPath / mx::FilePath("_options.mtlx");
    CrossShaderGeneratorTester tester(
        languageTarget == LanguageTarget::HLSL? mx::HLSLShaderGenerator::create() 
                                              : mx::SPIRVShaderGenerator::create(),
        testRootPaths, libSearchPath, srcSearchPath, logPath, writeShadersToDisk);
    tester.validate(genOptions, optionsFilePath);
}

TEST_CASE("GenShader: HLSL Shader Generation", "[gencross]")
{
    generateCrossCode(LanguageTarget::HLSL);
}

//TEST_CASE("GenShader: SPIRV Shader Generation", "[gencross]")
//{
//    generateCrossCode(LanguageTarget::SPIRV);
//}