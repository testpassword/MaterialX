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

SourceCode::SourceCode() :
    _indentations(0)
{
}

void SourceCode::beginScope(Syntax::Punctuation punc)
{
    switch (punc) {
    case Syntax::CURLY_BRACKETS:
        beginLine();
        _code += "{\n";
        break;
    case Syntax::PARENTHESES:
        beginLine();
        _code += "(\n";
        break;
    case Syntax::SQUARE_BRACKETS:
        beginLine();
        _code += "[\n";
        break;
    case Syntax::DOUBLE_SQUARE_BRACKETS:
        beginLine();
        _code += "[[\n";
        break;
    }

    ++_indentations;
    _scopes.push_back(punc);
}

void SourceCode::endScope(bool semicolon, bool newline)
{
    if (_scopes.empty())
    {
        throw ExceptionRuntimeError("End scope called with no scope active, please check your beginScope/endScope calls");
    }

    Syntax::Punctuation punc = _scopes.back();
    _scopes.pop_back();
    --_indentations;

    switch (punc) {
    case Syntax::CURLY_BRACKETS:
        beginLine();
        _code += "}";
        break;
    case Syntax::PARENTHESES:
        beginLine();
        _code += ")";
        break;
    case Syntax::SQUARE_BRACKETS:
        beginLine();
        _code += "]";
        break;
    case Syntax::DOUBLE_SQUARE_BRACKETS:
        beginLine();
        _code += "]]";
        break;
    }
    if (semicolon)
        _code += ";";
    if (newline)
        _code += "\n";
}

void SourceCode::beginLine()
{
    static const string INDENTATION = "    ";
    for (int i = 0; i < _indentations; ++i)
    {
        _code += INDENTATION;
    }
}

void SourceCode::endLine(bool semicolon)
{
    if (semicolon)
    {
        _code += ";";
    }
    newLine();
}

void SourceCode::newLine()
{
    _code += "\n";
}

void SourceCode::addString(const string& str)
{
    _code += str;
}

void SourceCode::addLine(const string& str, bool semicolon)
{
    beginLine();
    addString(str);
    endLine(semicolon);
}

void SourceCode::setDefined(const RtToken& function)
{
    _functions.insert(function);
}

bool SourceCode::isDefined(const RtToken& function) const
{
    return _functions.count(function) > 0;
}

void SourceCode::setIncluded(const RtToken& file)
{
    _includes.insert(file);
}

bool SourceCode::isIncluded(const RtToken& file) const
{
    return _includes.count(file) > 0;
}

const string& SourceCode::asString() const
{
    return _code;
}


void FragmentCompiler::emitFunctionCall(const Fragment& frag, SourceCode& result)
{
    // Declare all outputs.
    for (size_t i = 0; i < frag.numOutputs(); ++i)
    {
        const Fragment::Output* output = frag.getOutput(i);
        result.beginLine();
        declareVariable(*output, true, result);
        result.endLine();
    }

    result.beginLine();
    result.addString("void " + frag.getFunctionName().str() + "(");

    string delim = "";

    // Add all inputs
    for (size_t i = 0; i < frag.numInputs(); ++i)
    {
        const Fragment::Input* input = frag.getInput(i);
        result.addString(delim);
        emitVariable(*input, result);
        delim = ", ";
    }

    // Add all outputs
    for (size_t i = 0; i < frag.numOutputs(); ++i)
    {
        const Fragment::Output* output = frag.getOutput(i);
        result.addString(delim + output->variable.str());
        delim = ", ";
    }
    result.addString(")");
    result.endLine();
}

FragmentCompiler::FragmentCompiler(Context& context) :
    _context(context),
    _syntax(context.getSyntax())
{
}

void FragmentCompiler::compileFragments(const Fragment::Output* /*output*/, SourceCode& /*result*/)
{
}

void FragmentCompiler::compileFunction(const Fragment& frag, SourceCode& result)
{
    if (result.isDefined(frag.getFunctionName()))
    {
        return;
    }

    if (frag.getType() == FRAGMENT_TYPE_GRAPH)
    {
        const FragmentGraph& graph = static_cast<const FragmentGraph&>(frag);

        result.beginLine();
        result.addString("void " + graph.getFunctionName().str() + "(");

        string delim = "";

        // Add all inputs
        for (size_t i = 0; i < graph.numInputs(); ++i)
        {
            result.addString(delim);
            const Fragment::Input* input = graph.getInput(i);
            declareVariable(*input, false, result);
            delim = ", ";
        }

        // Add all outputs
        for (size_t i = 0; i < graph.numOutputs(); ++i)
        {
            result.addString(delim);
            const Fragment::Output* output = graph.getOutput(i);
            result.addString(_syntax.getOutputQualifier());
            declareVariable(*output, false, result);
            delim = ", ";
        }
        result.addString(")");
        result.endLine(false);

        result.beginScope();

        for (size_t i = 0; i < graph.numFragments(); ++i)
        {
            const Fragment* child = graph.getFragment(i);
            emitFunctionCall(*child, result);
        }

        // Emit final results
        for (size_t i = 0; i < graph.numOutputs(); ++i)
        {
            const Fragment::Input* outputSocket = graph.getOutputSocket(i);
            result.beginLine();
            result.addString(outputSocket->variable.str() + " = ");
            emitVariable(*outputSocket, result);
            result.endLine();
        }

        result.endScope();
    }
    else if (frag.getType() == FRAGMENT_TYPE_FUNCTION)
    {
        // Add function definition.
        emitBlock(frag.getSourceCode(), result);
        result.setDefined(frag.getFunctionName());
    }
}

void FragmentCompiler::compileShader(const Fragment& frag, SourceCode& result)
{
    compileFunction(frag, result);
}

void FragmentCompiler::declareVariable(const Fragment::Input& input, bool assignDefault, SourceCode& result)
{
    result.addString(_syntax.getTypeName(input.type) + " " + input.variable.str());
    if (assignDefault)
    {
        result.addString(" = " + _syntax.getDefaultValue(input.type));
    }
}

void FragmentCompiler::declareVariable(const Fragment::Output& output, bool assignDefault, SourceCode& result)
{
    result.addString(_syntax.getTypeName(output.type) + " " + output.variable.str());
    if (assignDefault)
    {
        result.addString(" = " + _syntax.getDefaultValue(output.type));
    }
}

void FragmentCompiler::emitBlock(const string& block, SourceCode& result)
{
    const string& INCLUDE = _syntax.getIncludeStatement();

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

void FragmentCompiler::emitInclude(const FilePath& file, SourceCode& result)
{
    const RtToken f(file);
    if (!result.isIncluded(f))
    {
        string modifiedFile = file;
        tokenSubstitution(_context.getSubstitutions(), modifiedFile);
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

void FragmentCompiler::emitVariable(const Fragment::Input& input, SourceCode& result)
{
    if (input.connection)
    {
        result.addString(input.connection->variable.str());
    }
    else
    {
        result.addString(_syntax.getValue(input.type, input.value));
    }
}

} // namespace Codegen
} // namespace MaterialX
