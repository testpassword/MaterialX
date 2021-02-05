//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXTest/Catch/catch.hpp>

#include <MaterialXCore/Document.h>

#include <MaterialXFormat/XmlIo.h>
#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <MaterialXRuntime/RtApi.h>
#include <MaterialXRuntime/RtValue.h>
#include <MaterialXRuntime/RtStage.h>
#include <MaterialXRuntime/RtPrim.h>
#include <MaterialXRuntime/RtRelationship.h>
#include <MaterialXRuntime/RtAttribute.h>
#include <MaterialXRuntime/RtNodeDef.h>
#include <MaterialXRuntime/RtTypeDef.h>
#include <MaterialXRuntime/RtNameResolver.h>
#include <MaterialXRuntime/RtNode.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtBackdrop.h>
#include <MaterialXRuntime/RtGeneric.h>
#include <MaterialXRuntime/RtPath.h>
#include <MaterialXRuntime/RtFileIo.h>
#include <MaterialXRuntime/RtTraversal.h>
#include <MaterialXRuntime/RtLook.h>
#include <MaterialXRuntime/RtCollection.h>
#include <MaterialXRuntime/RtMessage.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXRuntime/Codegen/CodeGenerator.h>
#include <MaterialXRuntime/Commands/PrimCommands.h>


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

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

TEST_CASE("Runtime: Codegen", "[runtime]")
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
    mx::Codegen::CodeGeneratorPtr gen = mx::Codegen::OslGenerator::create();

    mx::Codegen::CodegenOptionsPtr options = gen->createOptions();
    mx::Codegen::CodegenContextPtr context = gen->createContext(options);
    mx::Codegen::CodegenResultPtr result = gen->generate(node, "/", context);

    mx::Codegen::SourceCode sourceCode(gen->getSyntax());
    mx::Codegen::FragmentCompiler compiler;

    compiler.compileFunction(*context, *result->getFragment(0), sourceCode);

    sourceCode.newLine();

    compiler.compileFunctionCall(*context, *result->getFragment(0), sourceCode);

    std::cout << sourceCode.asString() << std::endl;
}
