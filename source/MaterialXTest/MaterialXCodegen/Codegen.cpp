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
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/OSL/OslContext.h>
#include <MaterialXCodegen/OSL/OslFragmentGenerator.h>

#include <iostream>
#include <fstream>

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
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath());
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/osl"));

    mx::RtScopedApiHandle api;
    api->setSearchPath(searchPath);

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr context = mx::Codegen::OslContext::create(options);

    const std::string addSource =
        "void add(float a, float b, output float result)\n"       \
        "{\n"                                                     \
        "   result = a + b;\n"                                    \
        "}\n";
    const std::string multiplySource =
        "void multiply(float a, float b, output float result)\n"  \
        "{\n"                                                     \
        "   result = a * b;\n"                                    \
        "}\n";

    // Test explicit fragment creation.
    mx::Codegen::FragmentPtr subgraphFragment = mx::Codegen::FragmentGraph::create(mx::RtToken("subgraph"));
    mx::Codegen::FragmentGraph* subgraph = subgraphFragment->asA<mx::Codegen::FragmentGraph>();
    subgraph->createInput(IN, mx::RtType::FLOAT);
    subgraph->createOutput(OUT, mx::RtType::FLOAT);
    {
        mx::Codegen::FragmentPtr multiply1 = mx::Codegen::SourceFragment::create(mx::RtToken("mult1"));
        multiply1->createInput(IN1, mx::RtType::FLOAT);
        multiply1->createInput(IN2, mx::RtType::FLOAT);
        multiply1->createOutput(OUT, mx::RtType::FLOAT);
        multiply1->setFunctionName(MULTIPLY);
        multiply1->asA<mx::Codegen::SourceFragment>()->setSourceCode(&multiplySource);

        mx::Codegen::FragmentPtr multiply2 = mx::Codegen::SourceFragment::create(mx::RtToken("mult2"));
        multiply2->createInput(IN1, mx::RtType::FLOAT);
        multiply2->createInput(IN2, mx::RtType::FLOAT);
        multiply2->createOutput(OUT, mx::RtType::FLOAT);
        multiply2->setFunctionName(MULTIPLY);
        multiply2->asA<mx::Codegen::SourceFragment>()->setSourceCode(&multiplySource);

        mx::Codegen::FragmentPtr add1 = mx::Codegen::SourceFragment::create(mx::RtToken("add1"));
        add1->createInput(IN1, mx::RtType::FLOAT);
        add1->createInput(IN2, mx::RtType::FLOAT);
        add1->createOutput(OUT, mx::RtType::FLOAT);
        add1->setFunctionName(ADD);
        add1->asA<mx::Codegen::SourceFragment>()->setSourceCode(&addSource);

        subgraph->addFragment(add1);
        subgraph->addFragment(multiply1);
        subgraph->addFragment(multiply2);

        multiply1->getInput(IN1)->getValue().asFloat() = 7.0f;
        subgraph->connect(subgraph->getInputSocket(IN), multiply1->getInput(IN2));
        subgraph->connect(multiply1->getOutput(), multiply2->getInput(IN1));
        subgraph->connect(multiply1->getOutput(), multiply2->getInput(IN2));
        subgraph->connect(multiply1->getOutput(), add1->getInput(IN1));
        subgraph->connect(multiply2->getOutput(), add1->getInput(IN2));
        subgraph->connect(add1->getOutput(), subgraph->getOutputSocket(OUT));

        subgraph->finalize(*context);
    }

    // Test fragment factory creation.
    const mx::Codegen::FragmentGenerator& generator = context->getGenerator();
    mx::Codegen::FragmentPtr maingraphFragment = generator.createFragment(mx::Codegen::FragmentGraph::className(), mx::RtToken("maingraph"));
    mx::Codegen::FragmentGraph* maingraph = maingraphFragment->asA<mx::Codegen::FragmentGraph>();
    maingraph->createInput(IN, mx::RtType::FLOAT);
    maingraph->createOutput(OUT, mx::RtType::FLOAT);
    {
        mx::Codegen::FragmentPtr multiply = generator.createFragment(mx::Codegen::SourceFragment::className(), mx::RtToken("mult"));
        multiply->createInput(IN1, mx::RtType::FLOAT);
        multiply->createInput(IN2, mx::RtType::FLOAT);
        multiply->createOutput(OUT, mx::RtType::FLOAT);
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

    const std::string filepath = mx::FilePath::getCurrentPath() / mx::FilePath("codegen_test1.osl");
    std::ofstream shaderFile;
    shaderFile.open(filepath);
    shaderFile << sourceCode.asString();
}

TEST_CASE("Codegen: Fragments from RtNodeGraph", "[codegen]")
{
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath());
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/osl"));

    mx::RtScopedApiHandle api;
    api->setSearchPath(searchPath);

    // Load in standard libraries.
    mx::RtReadOptions readOptions;
    api->loadLibrary(TARGETS, readOptions);
    api->loadLibrary(STDLIB, readOptions);
    api->loadLibrary(PBRLIB, readOptions);
    api->loadLibrary(BXDFLIB, readOptions);

    // Create a new working space stage
    mx::RtStagePtr stage = api->createStage(MAIN);

    mx::RtFileIo fileIO(stage);
    fileIO.read("resources/Materials/TestSuite/pbrlib/bsdf/layer_bsdf.mtlx", searchPath);

//    mx::RtPrim prim = stage->getPrimAtPath("/layer_bsdf_test2");

    mx::RtPrim prim = api->getLibrary()->getPrimAtPath("/IMPL_standard_surface_surfaceshader");
    REQUIRE(prim);

    mx::RtNodeGraph nodegraph(prim);

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr context = mx::Codegen::OslContext::create(options);

    const mx::Codegen::FragmentGenerator& generator = context->getGenerator();
    const mx::Codegen::FragmentCompiler& compiler = context->getCompiler();

    mx::Codegen::FragmentPtr frag = generator.createFragmentGraph(nodegraph);
    REQUIRE(frag);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileShader(*frag->getOutput(), sourceCode);

    const std::string filepath = mx::FilePath::getCurrentPath() / mx::FilePath("codegen_test2.osl");
    std::ofstream shaderFile;
    shaderFile.open(filepath);
    shaderFile << sourceCode.asString();
}

TEST_CASE("Codegen: Color Management", "[codegen]")
{
    mx::FileSearchPath searchPath(mx::FilePath::getCurrentPath());
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries"));
    searchPath.append(mx::FilePath::getCurrentPath() / mx::FilePath("libraries/stdlib/osl"));

    mx::RtScopedApiHandle api;
    api->setSearchPath(searchPath);

    // Load in standard libraries.
    mx::RtReadOptions readOptions;
    api->loadLibrary(TARGETS, readOptions);
    api->loadLibrary(STDLIB, readOptions);
    api->loadLibrary(PBRLIB, readOptions);
    api->loadLibrary(BXDFLIB, readOptions);

    mx::RtPrim prim = api->getLibrary()->getPrimAtPath("/NG_tiledimage_color3");
    REQUIRE(prim);

    mx::RtNodeGraph nodegraph(prim);
    mx::RtNode image = nodegraph.getNode(mx::RtToken("N_img_color3"));
    mx::RtInput file = image.getInput(mx::Tokens::FILE);
    file.setColorSpace(mx::RtToken("srgb_texture"));
    mx::RtInput default = image.getInput(mx::RtToken("default"));
    default.setColorSpace(mx::RtToken("gamma22"));
    default.clearConnection();

    mx::Codegen::OptionsPtr options = mx::Codegen::Options::create();
    mx::Codegen::ContextPtr context = mx::Codegen::OslContext::create(options);

    // Set target color space.
    options->targetColorSpaceOverride = mx::RtToken("lin_rec709");

    // Setup a color managment system.
    mx::Codegen::ColorManagementSystemPtr cms = mx::Codegen::DefaultColorManagementSystem::create(*context);
    const mx::FilePath cmsImplFile = searchPath.find("stdlib/genosl/stdlib_genosl_cm_impl.mtlx");
    cms->asA<mx::Codegen::DefaultColorManagementSystem>()->loadImplementations(cmsImplFile);
    context->setColorManagementSystem(cms);

    const mx::Codegen::FragmentGenerator& generator = context->getGenerator();
    const mx::Codegen::FragmentCompiler& compiler = context->getCompiler();

    mx::Codegen::FragmentPtr frag = generator.createFragmentGraph(nodegraph);
    REQUIRE(frag);

    mx::Codegen::SourceCode sourceCode;
    compiler.compileShader(*frag->getOutput(), sourceCode);

    const std::string filepath = mx::FilePath::getCurrentPath() / mx::FilePath("codegen_test3.osl");
    std::ofstream shaderFile;
    shaderFile.open(filepath);
    shaderFile << sourceCode.asString();
}
