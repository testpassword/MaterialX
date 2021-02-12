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

string& SourceCode::asString()
{
    return _code;
}


void FragmentCompiler::emitFunctionCall(const Fragment& frag, SourceCode& result) const
{
    if (frag.getFunctionName() != EMPTY_TOKEN)
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
        result.addString(frag.getFunctionName().str() + "(");

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
    else
    {
        // An inline expression.
        const string& source = frag.getSourceCode();
        const Syntax& syntax = _context.getSyntax();

        static const string prefix("{{");
        static const string postfix("}}");

        size_t pos = 0;
        size_t i = source.find_first_of(prefix);
        StringSet localVariableNames;
        SourceCode inlineResult;
        while (i != string::npos)
        {
            inlineResult.addString(source.substr(pos, i - pos));
            size_t j = source.find_first_of(postfix, i + 2);
            if (j == string::npos)
            {
                throw ExceptionRuntimeError("Malformed inline expression in source code for fragment " + frag.getName().str());
            }

            const RtToken variable(source.substr(i + 2, j - i - 2));
            const Fragment::Input* input = frag.getInput(variable);
            if (!input)
            {
                throw ExceptionRuntimeError("Could not find an input named '" + variable.str() + "' on fragment '" + frag.getName().str() + "'");
            }

            if (input->connection)
            {
                emitVariable(*input, inlineResult);
            }
            else
            {
                // Declare a local variable with this value.
                // TODO: We should make a valid unique identifier name.
                const string variableName = input->getLongName() + "_tmp";
                if (!localVariableNames.count(variableName))
                {
                    result.beginLine();
                    const string& qualifier = syntax.getConstantQualifier();
                    result.addString(qualifier.empty() ? EMPTY_STRING : qualifier + " ");
                    result.addString(syntax.getTypeName(input->type) + " " + input->variable.str() + " = " + syntax.getValue(input->type, input->value));
                    result.endLine();
                    localVariableNames.insert(variableName);
                }
                inlineResult.addString(variableName);
            }

            pos = j + 2;
            i = source.find_first_of(prefix, pos);
        }
        inlineResult.addString(source.substr(pos));

        result.beginLine();
        declareVariable(*frag.getOutput(), false, result);
        result.addString(" = " + inlineResult.asString());
        result.endLine();
    }
}

FragmentCompiler::FragmentCompiler(const Context& context) :
    _context(context)
{
}

void FragmentCompiler::compileFunction(const Fragment& frag, SourceCode& result) const
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
            result.addString(_context.getSyntax().getOutputQualifier());
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

void FragmentCompiler::declareVariable(const Fragment::Port& port, bool assignDefault, SourceCode& result) const
{
    const Syntax& syntax = _context.getSyntax();
    result.addString(syntax.getTypeName(port.type) + " " + port.variable.str());
    if (assignDefault)
    {
        result.addString(" = " + syntax.getDefaultValue(port.type));
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

void FragmentCompiler::emitVariable(const Fragment::Input& input, SourceCode& result) const
{
    if (input.connection)
    {
        result.addString(input.connection->variable.str());
    }
    else
    {
        result.addString(_context.getSyntax().getValue(input.type, input.value));
    }
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


} // namespace Codegen
} // namespace MaterialX
