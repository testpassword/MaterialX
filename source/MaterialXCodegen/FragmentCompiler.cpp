//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtApi.h>

#include <MaterialXFormat/Util.h>

#include <MaterialXGenShader/Util.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

FragmentCompiler::FragmentCompiler(const Context& context) :
    _context(context)
{
}

void FragmentCompiler::declareVariable(const Attribute& attr, bool assignDefault, SourceCode& result) const
{
    const Syntax& syntax = _context.getSyntax();
    result.addString(syntax.getTypeName(attr.getType()) + " " + attr.getVariable().str());
    if (assignDefault)
    {
        result.addString(" = " + syntax.getDefaultValue(attr.getType()));
    }
}

void FragmentCompiler::emitBlock(const string& block, SourceCode& result) const
{
    const string& INCLUDE = _context.getSyntax().getIncludeStatement();

    // Add each line in the block seperatelly
    // to get correct indentation
    std::stringstream stream(block);
    for (string line; std::getline(stream, line); )
    {
        const size_t pos = line.find(INCLUDE);
        if (pos != string::npos)
        {
            const size_t startQuote = line.find_first_of('\"');
            const size_t endQuote = line.find_last_of('\"');
            if (startQuote != string::npos && endQuote != string::npos && endQuote > startQuote)
            {
                size_t length = (endQuote - startQuote) - 1;
                if (length)
                {
                    const string filename = line.substr(startQuote + 1, length);
                    emitInclude(filename, result);
                }
            }
        }
        else
        {
            result.addLine(line, false);
        }
    }
}

void FragmentCompiler::emitInclude(const FilePath& file, SourceCode& result) const
{
    const RtToken f(file);
    if (!result.isIncluded(f))
    {
        string modifiedFile = file;
        tokenSubstitution(_substitutions, modifiedFile);
        const FilePath resolvedFile = RtApi::get().getSearchPath().find(modifiedFile);
        const string content = readFile(resolvedFile);
        if (content.empty())
        {
            throw ExceptionRuntimeError("Could not find include file: '" + resolvedFile.asString() + "'");
        }
        emitBlock(content, result);
        result.setIncluded(f);
    }
}

string FragmentCompiler::getResult(const Input& input) const
{
    return input.isConnected() ? input.getConnection()->getVariable().str() :
        _context.getSyntax().getValue(input.getType(), input.getValue());
}

void FragmentCompiler::emitTypeDefinitions(SourceCode& result) const
{
    for (const auto& syntax : _context.getSyntax().getTypeSyntaxes())
    {
        if (!syntax->getTypeAlias().empty())
        {
            result.addLine("#define " + syntax->getTypeName() + " " + syntax->getTypeAlias(), false);
        }
        if (!syntax->getTypeDefinition().empty())
        {
            result.addLine(syntax->getTypeDefinition(), false);
        }
    }
    result.newLine();
}

void FragmentCompiler::emitFunctionDefinitions(const FragmentGraph& graph, SourceCode& result) const
{
    // Default implementation, just emit all function definitions for all fragments.
    for (size_t i = 0; i < graph.numFragments(); ++i)
    {
        const Fragment* child = graph.getFragment(i);
        child->emitFunctionDefinitions(_context, result);
    }
}

void FragmentCompiler::emitFunctionCalls(const FragmentGraph& graph, SourceCode& result) const
{
    // Default implementation, just emit all function calls for all fragments.
    for (size_t i = 0; i < graph.numFragments(); ++i)
    {
        const Fragment* child = graph.getFragment(i);
        child->emitFunctionCall(_context, result);
    }
}

} // namespace Codegen
} // namespace MaterialX
