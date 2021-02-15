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
    const mx::RtToken IN("in");
    const mx::RtToken IN1("in1");
    const mx::RtToken IN2("in2");
    const mx::RtToken OUT("out");
}

TEST_CASE("Codegen: Fragments from source", "[codegen]")
{
    mx::RtScopedApiHandle api;
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));
    api->setSearchPath(searchPath);

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr context = mx::Codegen::OslContext::create(options);

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

    mx::Codegen::FragmentPtr subgraphFragment = mx::Codegen::FragmentGraph::create(mx::RtToken("subgraph"));
    mx::Codegen::FragmentGraph* subgraph = subgraphFragment->asA<mx::Codegen::FragmentGraph>();
    subgraph->createInput(mx::RtType::FLOAT, IN);
    subgraph->createOutput(mx::RtType::FLOAT, OUT);
    {
        mx::Codegen::FragmentPtr multiply1 = mx::Codegen::SourceFragment::create(mx::RtToken("mult1"));
        multiply1->createInput(mx::RtType::FLOAT, IN1);
        multiply1->createInput(mx::RtType::FLOAT, IN2);
        multiply1->createOutput(mx::RtType::FLOAT, OUT);
        multiply1->setFunctionName(MULTIPLY);
        multiply1->asA<mx::Codegen::SourceFragment>()->setSourceCode(&multiplySource);

        mx::Codegen::FragmentPtr multiply2 = mx::Codegen::SourceFragment::create(mx::RtToken("mult2"));
        multiply2->createInput(mx::RtType::FLOAT, IN1);
        multiply2->createInput(mx::RtType::FLOAT, IN2);
        multiply2->createOutput(mx::RtType::FLOAT, OUT);
        multiply2->setFunctionName(MULTIPLY);
        multiply2->asA<mx::Codegen::SourceFragment>()->setSourceCode(&multiplySource);

        mx::Codegen::FragmentPtr add1 = mx::Codegen::SourceFragment::create(mx::RtToken("add1"));
        add1->createInput(mx::RtType::FLOAT, IN1);
        add1->createInput(mx::RtType::FLOAT, IN2);
        add1->createOutput(mx::RtType::FLOAT, OUT);
        add1->setFunctionName(ADD);
        add1->asA<mx::Codegen::SourceFragment>()->setSourceCode(&addSource);

        subgraph->addFragment(add1);
        subgraph->addFragment(multiply1);
        subgraph->addFragment(multiply2);

        multiply1->getInput(IN1)->value.asFloat() = 7.0f;
        subgraph->connect(subgraph->getInputSocket(IN), multiply1->getInput(IN2));
        subgraph->connect(multiply1->getOutput(), multiply2->getInput(IN1));
        subgraph->connect(multiply1->getOutput(), multiply2->getInput(IN2));
        subgraph->connect(multiply1->getOutput(), add1->getInput(IN1));
        subgraph->connect(multiply2->getOutput(), add1->getInput(IN2));
        subgraph->connect(add1->getOutput(), subgraph->getOutputSocket(OUT));

        subgraph->finalize(*context);
    }

    mx::Codegen::FragmentPtr maingraphFragment = mx::Codegen::FragmentGraph::create(mx::RtToken("maingraph"));
    mx::Codegen::FragmentGraph* maingraph = maingraphFragment->asA<mx::Codegen::FragmentGraph>();
    maingraph->createInput(mx::RtType::FLOAT, IN);
    maingraph->createOutput(mx::RtType::FLOAT, OUT);
    {
        mx::Codegen::FragmentPtr multiply = mx::Codegen::SourceFragment::create(mx::RtToken("mult"));
        multiply->createInput(mx::RtType::FLOAT, IN1);
        multiply->createInput(mx::RtType::FLOAT, IN2);
        multiply->createOutput(mx::RtType::FLOAT, OUT);
        multiply->setFunctionName(MULTIPLY);
        multiply->asA<mx::Codegen::SourceFragment>()->setSourceCode(&multiplySource);

        maingraph->addFragment(multiply);
        maingraph->addFragment(subgraphFragment);

        maingraph->connect(maingraph->getInputSocket(IN), subgraph->getInput(IN));
        maingraph->connect(subgraph->getOutput(), multiply->getInput(IN1));
        maingraph->connect(subgraph->getOutput(), multiply->getInput(IN2));
        maingraph->connect(multiply->getOutput(), maingraph->getOutputSocket(OUT));

        maingraph->finalize(*context);
    }

    const mx::Codegen::FragmentCompiler& compiler = context->getCompiler();

    mx::Codegen::SourceCode sourceCode;
    compiler.compileShader(*maingraph->getOutput(), sourceCode);

    std::cout << sourceCode.asString() << std::endl;
}

TEST_CASE("Codegen: Fragments from nodes", "[codegen]")
{
    mx::RtScopedApiHandle api;
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));
    api->setSearchPath(searchPath);

    // Load in standard libraries.
    mx::RtReadOptions readOptions;
    api->loadLibrary(TARGETS, readOptions);
    api->loadLibrary(STDLIB, readOptions);
    api->loadLibrary(PBRLIB, readOptions);
    api->loadLibrary(BXDFLIB, readOptions);

//    mx::RtPrim graph = api->getLibrary()->getPrimAtPath("/IMPL_standard_surface_surfaceshader");
    mx::RtPrim graph = api->getLibrary()->getPrimAtPath("/NG_tiledimage_float");
    REQUIRE(graph);

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr contex = mx::Codegen::OslContext::create(options);

    const mx::Codegen::FragmentGenerator& generator = contex->getGenerator();
    const mx::Codegen::FragmentCompiler& compiler = contex->getCompiler();

    mx::Codegen::FragmentPtr frag = generator.createFragment(graph);
    REQUIRE(frag);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileShader(*frag->getOutput(), sourceCode);

//    std::cout << sourceCode.asString() << std::endl;
}
