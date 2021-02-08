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

TEST_CASE("Codegen: Fragments from source", "[codegen]")
{
    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);
    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();

    const std::string multiplySource =
        "void multiply(float a, float b, output float result)\n"  \
        "{\n"                                                     \
        "   result = a + b;\n"                                    \
        "}\n";

    mx::Codegen::FragmentPtr multiply1 = generator.createFragment(mx::RtToken("mult1"));
    multiply1->createInput(mx::RtType::FLOAT, IN1);
    multiply1->createInput(mx::RtType::FLOAT, IN2);
    multiply1->createOutput(mx::RtType::FLOAT, OUT);
    multiply1->setSourceCodeFunction(MULTIPLY, multiplySource);

    mx::Codegen::FragmentPtr multiply2 = generator.createFragment(mx::RtToken("mult2"));
    multiply2->createInput(mx::RtType::FLOAT, IN1);
    multiply2->createInput(mx::RtType::FLOAT, IN2);
    multiply2->createOutput(mx::RtType::FLOAT, OUT);
    multiply2->setSourceCodeFunction(MULTIPLY, multiplySource);

    mx::Codegen::Fragment::Input* multiply1_in1 = multiply1->getInput(0);
    mx::Codegen::Fragment::Input* multiply1_in2 = multiply1->getInput(1);
    mx::Codegen::Fragment::Output* multiply1_out = multiply1->getOutput(0);
    mx::Codegen::Fragment::Input* multiply2_in1 = multiply2->getInput(0);
    mx::Codegen::Fragment::Input* multiply2_in2 = multiply2->getInput(1);
    multiply1_in1->value.asFloat() = 3.0f;
    multiply1_in2->value.asFloat() = 7.0f;
    multiply2_in1->connection = multiply1_out;
    multiply2_in2->connection = multiply1_out;

    mx::Codegen::FragmentCompiler compiler(*contex);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileFunction(*multiply1, sourceCode);
    compiler.compileFunction(*multiply2, sourceCode);
    compiler.compileFunctionCall(*multiply1, sourceCode);
    compiler.compileFunctionCall(*multiply2, sourceCode);

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

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);

    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();
    mx::Codegen::FragmentPtr frag = generator.createFragment(node);

    mx::Codegen::FragmentCompiler compiler(*contex);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileFunction(*frag, sourceCode);
    sourceCode.newLine();
    compiler.compileFunctionCall(*frag, sourceCode);
    std::cout << sourceCode.asString() << std::endl;
}
