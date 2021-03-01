//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslFragmentCompiler.h>
#include <MaterialXCodegen/OSL/OslSyntax.h>
#include <MaterialXCodegen/OSL/OslContext.h>

#include <MaterialXRuntime/RtApi.h>

namespace MaterialX
{
namespace Codegen
{

OslFragmentCompiler::OslFragmentCompiler(const Context& context) :
    FragmentCompiler(context)
{
    // Add reserved words from the syntax
    _reservedWords = context.getSyntax().getReservedWords();

    // Add token substitution identifiers
    for (const auto& it : _substitutions)
    {
        if (!it.second.empty())
        {
            _reservedWords.insert(RtToken(it.second));
        }
    }

    // Set the include file to use for uv transformations,
    // depending on the vertical flip flag.
    static const string T_FILE_TRANSFORM_UV = "$fileTransformUv";
    if (context.getOptions().fileTextureVerticalFlip)
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + OslContext::TARGET.str() + "/lib/mx_transform_uv_vflip.osl";
    }
    else
    {
        _substitutions[T_FILE_TRANSFORM_UV] = "stdlib/" + OslContext::TARGET.str() + "/lib/mx_transform_uv.osl";
    }
}

FragmentCompilerPtr OslFragmentCompiler::create(const Context& context)
{
    return FragmentCompilerPtr(new OslFragmentCompiler(context));
}

void OslFragmentCompiler::compileShader(const Output& output, SourceCode& result) const
{
    if (output.getParent()->getClassName() != FragmentGraph::className())
    {
        throw ExceptionRuntimeError("Given output is not a fragment graph output '" + output.getName().str() + "'");
    }

    const FragmentCompiler& compiler = _context.getCompiler();
    const Syntax& syntax = _context.getSyntax();
    const FragmentGraph* graph = output.getParent()->asA<FragmentGraph>();

    const FilePath includeFile = RtApi::get().getSearchPath().find("mx_funcs.h");
    const FilePath albedoTableFile = RtApi::get().getSearchPath().find("resources/Lights/AlbedoTable.exr");
    
    // Add include files.
    result.addLine("#include \"" + includeFile.asString(FilePath::FormatPosix) + "\"", false);

    // Add global constants and type definitions.
    emitTypeDefinitions(result);
    result.addLine("#define true 1", false);
    result.addLine("#define false 0", false);
    result.addLine("#define M_FLOAT_EPS 1e-8", false);
    result.addLine("#define M_GOLDEN_RATIO 1.6180339887498948482045868343656", false);
    result.addLine("#define GGX_DIRECTIONAL_ALBEDO_METHOD " + std::to_string(int(_context.getOptions().directionalAlbedoMethod)), false);
    result.addLine("#define GGX_DIRECTIONAL_ALBEDO_TABLE \"" + albedoTableFile.asString(FilePath::FormatPosix) + "\"", false);
    result.newLine();

    // Emit all function definitions.
    compiler.emitFunctionDefinitions(*graph, result);

    // Begin shader signature.
    if (output.getType() == RtType::SURFACESHADER)
    {
        result.addString("surface ");
    }
    else if (output.getType() == RtType::VOLUMESHADER)
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
        const Input* port = graph->getInput(i);
        result.addLine(syntax.getTypeName(port->getType()) + " " + port->getVariable().str() + " = " +
            syntax.getInterfaceValue(port->getType(), port->getValue()) + ',', false);
    }
    for (size_t i = 0; i < graph->numOutputs(); ++i)
    {
        const Output* port = graph->getOutput(i);
        const string delim = (i == graph->numOutputs() - 1) ? EMPTY_STRING : ",";
        result.addLine(syntax.getOutputQualifier() + syntax.getTypeName(port->getType()) + " " + 
            port->getVariable().str() + " = " + syntax.getInterfaceDefaultValue(port->getType()) + delim, false);
    }
    result.endScope();

    // Emit shader body.
    result.beginScope();

    // Emit all function calls.
    compiler.emitFunctionCalls(*graph, result);

    // Emit final results
    const Input* outputSocket = graph->getOutputSocket(output.getName());
    result.beginLine();
    result.addString(outputSocket->getVariable().str() + " = ");
    result.addString(getResult(*outputSocket));
    result.endLine();

    result.endScope();
}

void OslFragmentCompiler::emitFunctionCalls(const FragmentGraph& graph, SourceCode& result) const
{
    if (graph.hasClassification(FragmentClassification::CLOSURE) ||
        graph.hasClassification(FragmentClassification::SHADER))
    {
        result.addLine("closure color null_closure = 0");
    }
    FragmentCompiler::emitFunctionCalls(graph, result);
}

} // namespace Codegen
} // namespace MaterialX
