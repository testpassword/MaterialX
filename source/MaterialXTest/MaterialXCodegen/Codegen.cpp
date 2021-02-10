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
    const mx::RtToken ADD("add");
    const mx::RtToken IN1("in1");
    const mx::RtToken IN2("in2");
    const mx::RtToken OUT("out");
}

TEST_CASE("Codegen: Fragments from source", "[codegen]")
{
    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);
    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();

    const std::string addSource =
        "void add(float a, float b, output float result)\n"  \
        "{\n"                                                     \
        "   result = a + b;\n"                                    \
        "}\n";
    const std::string multiplySource =
        "void multiply(float a, float b, output float result)\n"  \
        "{\n"                                                     \
        "   result = a * b;\n"                                    \
        "}\n";

    mx::Codegen::FragmentPtr multiply1 = generator.createFragment(mx::RtToken("mult1"));
    multiply1->createInput(mx::RtType::FLOAT, IN1);
    multiply1->createInput(mx::RtType::FLOAT, IN2);
    multiply1->createOutput(mx::RtType::FLOAT, OUT);
    multiply1->setFunctionName(MULTIPLY);
    multiply1->setSourceCode(multiplySource);

    mx::Codegen::FragmentPtr multiply2 = generator.createFragment(mx::RtToken("mult2"));
    multiply2->createInput(mx::RtType::FLOAT, IN1);
    multiply2->createInput(mx::RtType::FLOAT, IN2);
    multiply2->createOutput(mx::RtType::FLOAT, OUT);
    multiply2->setFunctionName(MULTIPLY);
    multiply2->setSourceCode(multiplySource);

    mx::Codegen::FragmentPtr add1 = generator.createFragment(mx::RtToken("add1"));
    add1->createInput(mx::RtType::FLOAT, IN1);
    add1->createInput(mx::RtType::FLOAT, IN2);
    add1->createOutput(mx::RtType::FLOAT, OUT);
    add1->setFunctionName(ADD);
    add1->setSourceCode(addSource);

    mx::Codegen::FragmentGraphPtr graph = generator.createFragmentGraph(mx::RtToken("testgraph"));
    graph->addFragment(add1);
    graph->addFragment(multiply1);
    graph->addFragment(multiply2);

    multiply1->getInput(IN1)->value.asFloat() = 3.0f;
    multiply1->getInput(IN2)->value.asFloat() = 7.0f;
    graph->connect(multiply1->getOutput(), multiply2->getInput(IN1));
    graph->connect(multiply1->getOutput(), multiply2->getInput(IN2));
    graph->connect(multiply1->getOutput(), add1->getInput(IN1));
    graph->connect(multiply2->getOutput(), add1->getInput(IN2));

    graph->createOutput(mx::RtType::FLOAT, OUT);
    graph->connect(add1->getOutput(), graph->getOutputSocket(OUT));

    graph->finalize();

    mx::Codegen::FragmentCompiler compiler(*contex);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileFunction(*graph, sourceCode);

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

    mx::RtPrim graph = api->getLibrary()->getPrimAtPath("/NG_tiledimage_float");
    REQUIRE(graph);

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);

    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();
    mx::Codegen::FragmentPtr frag = generator.createFragment(graph);
    REQUIRE(frag);

    mx::Codegen::FragmentCompiler compiler(*contex);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileFunction(*frag, sourceCode);
    sourceCode.newLine();
    compiler.emitFunctionCall(*frag, sourceCode);

    std::cout << sourceCode.asString() << std::endl;
}
