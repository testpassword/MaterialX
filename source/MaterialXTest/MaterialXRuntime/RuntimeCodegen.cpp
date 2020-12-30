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

#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>


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

    // Load in stdlib
    api->setSearchPath(searchPath);
    mx::RtReadOptions readOptions;
    readOptions.applyFutureUpdates = true;
    api->loadLibrary(TARGETS, readOptions);
    api->loadLibrary(STDLIB, readOptions);
    api->loadLibrary(PBRLIB, readOptions);
    api->loadLibrary(BXDFLIB, readOptions);

    // Create a stage.
    mx::RtStagePtr stage = api->createStage(MAIN);

    mx::RtPrim graph = api->getLibrary()->getPrimAtPath("/NG_tiledimage_float");
    REQUIRE(graph);

    mx::RtCodeGeneratorPtr gen = mx::RtOslGenerator::create();

    mx::RtCodegenOptionsPtr options = gen->createOptions();
    mx::RtCodegenContextPtr context = gen->createContext(options);
    mx::RtCodegenResultPtr result = gen->generate(graph , "/", context);

    std::cout << result->getFragment(0)->getSourceCode() << std::endl;
}
