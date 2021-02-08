//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXRuntime/RtApi.h>
#include <MaterialXRuntime/RtStage.h>
#include <MaterialXRuntime/RtPrim.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>

#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/OSL/OslGenerator.h>

#include <iostream>

namespace mx = MaterialX;

namespace
{
    // Commonly used tokens.
    const mx::RtToken TARGETS("targets");
    const mx::RtToken STDLIB("stdlib");
    const mx::RtToken PBRLIB("pbrlib");
    const mx::RtToken BXDFLIB("bxdf");
    const mx::RtToken ADSKLIB("adsk");
    const mx::RtToken MAIN("main");
    const mx::RtToken MULTIPLY("multiply");
    const mx::RtToken MULTIPLY_INLINE("multiply_inline");
    const mx::RtToken IN1("in1");
    const mx::RtToken IN2("in2");
    const mx::RtToken OUT("out");
}

TEST_CASE("Codegen: Fragments from code", "[codegen]")
{
    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);

    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();

    mx::Codegen::FragmentPtr multiply1 = generator.createFragment(MULTIPLY);
    mx::Codegen::Fragment::Input* in1 = multiply1->createInput(mx::RtType::FLOAT, IN1);
    mx::Codegen::Fragment::Input* in2 = multiply1->createInput(mx::RtType::FLOAT, IN2);
    mx::Codegen::Fragment::Output* out = multiply1->createOutput(mx::RtType::FLOAT, OUT);
    multiply1->setSourceCode(
        "void multiply(float a, float b, output float result)\n"  \
        "{\n"                                                     \
        "   result = a + b;\n"                                    \
        "}\n"                                                     \
    );

    mx::Codegen::FragmentCompiler compiler(*contex);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileFunction(*multiply1, sourceCode);
    sourceCode.newLine();
    compiler.compileFunctionCall(*multiply1, sourceCode);
    std::cout << sourceCode.asString() << std::endl;
}

TEST_CASE("Codegen: Fragments from nodes", "[codegen]")
{
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));

    mx::RtScopedApiHandle api;

    // Load in standard libraries.
    api->setSearchPath(searchPath);
    mx::RtReadOptions readOptions;
    api->loadLibrary(TARGETS, readOptions);
    api->loadLibrary(STDLIB, readOptions);
    api->loadLibrary(PBRLIB, readOptions);
    api->loadLibrary(BXDFLIB, readOptions);

    // Create a stage.
    mx::RtStagePtr stage = api->createStage(MAIN);
    mx::RtPrim node = stage->createPrim(mx::RtToken("ND_artistic_ior"));
/*
    mx::RtPrim graph = api->getLibrary()->getPrimAtPath("/NG_tiledimage_float");
    REQUIRE(graph);
    mx::RtCommandResult cmdResult;
    mx::RtCommand::copyPrim(stage, graph, cmdResult);
    mx::RtPrim newGraph = cmdResult.getObject();
*/
}
