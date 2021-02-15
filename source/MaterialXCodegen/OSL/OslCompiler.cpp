//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslCompiler.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslContext.h>

#include <MaterialXRuntime/RtApi.h>

namespace MaterialX
{
namespace Codegen
{

OslCompiler::OslCompiler(const Context& context) :
    FragmentCompiler(context)
{
}

FragmentCompilerPtr OslCompiler::create(const Context& context)
{
    return FragmentCompilerPtr(new OslCompiler(context));
}

void OslCompiler::compileShader(const Fragment::Output& output, SourceCode& result) const
{
    if (output.parent->getType() != FRAGMENT_TYPE_GRAPH)
    {
        throw ExceptionRuntimeError("Given output is not a fragment graph output '" + output.name.str() + "'");
    }

    const Syntax& syntax = _context.getSyntax();
    const FragmentGraph* graph = output.parent->asA<FragmentGraph>();

    const FilePath includeFile = RtApi::get().getSearchPath().find("mx_funcs.h");
    const FilePath albedoTableFile = RtApi::get().getSearchPath().find("resources/Lights/AlbedoTable.exr");
    
    // Add include files.
    result.addLine("#include \"" + includeFile.asString(FilePath::FormatPosix) + "\"");

    // Add global constants and type definitions.
    emitTypeDefinitions(result);
    result.addLine("#define M_FLOAT_EPS 1e-8", false);
    result.addLine("#define M_GOLDEN_RATIO 1.6180339887498948482045868343656", false);
    result.addLine("#define GGX_DIRECTIONAL_ALBEDO_METHOD " + std::to_string(int(_context.getOptions().directionalAlbedoMethod)), false);
    result.addLine("#define GGX_DIRECTIONAL_ALBEDO_TABLE \"" + albedoTableFile.asString(FilePath::FormatPosix) + "\"", false);
    result.newLine();

    // Emit all function definitions.
    for (size_t i = 0; i < graph->numFragments(); ++i)
    {
        const Fragment* child = graph->getFragment(i);
        child->emitFunctionDefinitions(_context, result);
    }

    // Begin shader signature.
    if (output.type == RtType::SURFACESHADER)
    {
        result.addString("surface ");
    }
    else if (output.type == RtType::VOLUMESHADER)
    {
        result.addString("volume ");
    }
    else
    {
        result.addString("shader ");
    }
    result.addString(graph->getFunctionName().str());
    result.newLine();

    // Add all inputs and outputs
    result.beginScope(Syntax::PARENTHESES);
    for (size_t i = 0; i < graph->numInputs(); ++i)
    {
        const Fragment::Input* port = graph->getInput(i);
        result.addLine(syntax.getTypeName(port->type) + " " + port->variable.str() + " = " +
            syntax.getValue(port->type, port->value) + ',', false);
    }
    for (size_t i = 0; i < graph->numOutputs(); ++i)
    {
        const Fragment::Output* port = graph->getOutput(i);
        const string delim = (i == graph->numOutputs() - 1) ? EMPTY_STRING : ",";
        result.addLine(syntax.getOutputQualifier() + syntax.getTypeName(port->type) + " " + 
            port->variable.str() + " = " + syntax.getDefaultValue(port->type) + delim, false);
    }
    result.endScope();

    // Emit shader body.
    result.beginScope();

    if (graph->isClass(FragmentClass::CLOSURE))
    {
        result.addLine("closure color null_closure = 0");
    }

    for (size_t i = 0; i < graph->numFragments(); ++i)
    {
        const Fragment* child = graph->getFragment(i);
        child->emitFunctionCall(_context, result);
    }

    // Emit final results
    const Fragment::Input* outputSocket = graph->getOutputSocket(output.name);
    result.beginLine();
    result.addString(outputSocket->variable.str() + " = ");
    emitVariable(*outputSocket, result);
    result.endLine();

    result.endScope();
}

} // namespace Codegen
} // namespace MaterialX
