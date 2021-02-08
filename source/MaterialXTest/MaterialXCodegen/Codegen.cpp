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
}

TEST_CASE("Codegen: Fragments", "[codegen]")
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
    mx::Codegen::FragmentGeneratorPtr gen = mx::Codegen::OslGenerator::create();

    mx::Codegen::OptionsPtr options = gen->createOptions();
    mx::Codegen::ContextPtr context = gen->createContext(options);


    mx::Codegen::FragmentPtr frag = gen->createFragment(node);

    mx::Codegen::SourceCode sourceCode(gen->getSyntax());
    mx::Codegen::FragmentCompiler compiler;
    compiler.compileFunction(*context, *frag, sourceCode);

    sourceCode.newLine();

    compiler.compileFunctionCall(*context, *frag, sourceCode);

    std::cout << sourceCode.asString() << std::endl;
}
