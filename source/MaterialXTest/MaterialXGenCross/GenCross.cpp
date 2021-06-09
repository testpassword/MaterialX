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

static void generateSPIRVCode()
{
    const mx::FilePath testRootPath = mx::FilePath::getCurrentPath() / mx::FilePath("resources/Materials/TestSuite");
    const mx::FilePath testRootPath2 = mx::FilePath::getCurrentPath() / mx::FilePath("resources/Materials/Examples/StandardSurface");
    mx::FilePathVec testRootPaths;
    testRootPaths.push_back(testRootPath2);
    const mx::FilePath libSearchPath = mx::FilePath::getCurrentPath() / mx::FilePath("libraries");
    const mx::FileSearchPath srcSearchPath(libSearchPath.asString());
    bool writeShadersToDisk = false;

    const mx::FilePath logPath("genspirv_generate_test.txt");
    const mx::GenOptions genOptions;
    mx::FilePath optionsFilePath = testRootPath / mx::FilePath("_options.mtlx");
    SPIRVShaderGeneratorTester tester(mx::SPIRVShaderGenerator::create(), testRootPaths, libSearchPath, srcSearchPath, logPath, writeShadersToDisk);
    tester.validate(genOptions, optionsFilePath);
}

TEST_CASE("GenShader: SPIRV Shader Generation", "[genspirv]")
{
    generateSPIRVCode();
}