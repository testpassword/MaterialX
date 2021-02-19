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

FragmentCompilerPtr OslCompiler::create(const Context& context)
{
    return FragmentCompilerPtr(new OslCompiler(context));
}

void OslCompiler::compileShader(const Output& output, SourceCode& result) const
{
    if (output.getParent()->getClassName() != FragmentGraph::className())
    {
        throw ExceptionRuntimeError("Given output is not a fragment graph output '" + output.getName().str() + "'");
    }

    const Syntax& syntax = _context.getSyntax();
    const FragmentGraph* graph = output.getParent()->asA<FragmentGraph>();

    const FilePath includeFile = RtApi::get().getSearchPath().find("mx_funcs.h");
    const FilePath albedoTableFile = RtApi::get().getSearchPath().find("resources/Lights/AlbedoTable.exr");
    
    // Add include files.
    result.addLine("#include \"" + includeFile.asString(FilePath::FormatPosix) + "\"", false);

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
            syntax.getValue(port->getType(), port->getValue()) + ',', false);
    }
    for (size_t i = 0; i < graph->numOutputs(); ++i)
    {
        const Output* port = graph->getOutput(i);
        const string delim = (i == graph->numOutputs() - 1) ? EMPTY_STRING : ",";
        result.addLine(syntax.getOutputQualifier() + syntax.getTypeName(port->getType()) + " " + 
            port->getVariable().str() + " = " + syntax.getDefaultValue(port->getType()) + delim, false);
    }
    result.endScope();

    // Emit shader body.
    result.beginScope();

    if (graph->hasClassification(FragmentClassification::CLOSURE))
    {
        result.addLine("closure color null_closure = 0");
    }

    for (size_t i = 0; i < graph->numFragments(); ++i)
    {
        const Fragment* child = graph->getFragment(i);
        child->emitFunctionCall(_context, result);
    }

    // Emit final results
    const Input* outputSocket = graph->getOutputSocket(output.getName());
    result.beginLine();
    result.addString(outputSocket->getVariable().str() + " = ");
    result.addString(getResult(*outputSocket));
    result.endLine();

    result.endScope();
}

} // namespace Codegen
} // namespace MaterialX
