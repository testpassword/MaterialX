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

SourceCode::SourceCode(const Syntax& syntax) :
    _syntax(syntax),
    _indentations(0)
{
}

void SourceCode::beginScope(Syntax::Punctuation punc)
{
    switch (punc) {
    case Syntax::CURLY_BRACKETS:
        beginLine();
        _code += "{" + _syntax.getNewline();
        break;
    case Syntax::PARENTHESES:
        beginLine();
        _code += "(" + _syntax.getNewline();
        break;
    case Syntax::SQUARE_BRACKETS:
        beginLine();
        _code += "[" + _syntax.getNewline();
        break;
    case Syntax::DOUBLE_SQUARE_BRACKETS:
        beginLine();
        _code += "[[" + _syntax.getNewline();
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
        _code += _syntax.getNewline();
}

void SourceCode::beginLine()
{
    for (int i = 0; i < _indentations; ++i)
    {
        _code += _syntax.getIndentation();
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
    _code += _syntax.getNewline();
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

void SourceCode::addComment(const string& str)
{
    beginLine();
    _code += _syntax.getSingleLineComment() + str;
    endLine(false);
}

void SourceCode::addBlock(const string& str)
{
    const string& INCLUDE = _syntax.getIncludeStatement();
    const string& QUOTE = _syntax.getStringQuote();

    // Add each line in the block seperatelly
    // to get correct indentation
    std::stringstream stream(str);
    for (string line; std::getline(stream, line); )
    {
        const size_t pos = line.find(INCLUDE);
        if (pos != string::npos)
        {
            const size_t startQuote = line.find_first_of(QUOTE);
            const size_t endQuote = line.find_last_of(QUOTE);
            if (startQuote != string::npos && endQuote != string::npos && endQuote > startQuote)
            {
                size_t length = (endQuote - startQuote) - 1;
                if (length)
                {
                    const string filename = line.substr(startQuote + 1, length);
                    addInclude(filename);
                }
            }
        }
        else
        {
            addLine(line, false);
        }
    }
}

void SourceCode::addInclude(const string& file)
{
    if (!_includes.count(file))
    {
        string content = readFile(file);
        if (content.empty())
        {
            throw ExceptionRuntimeError("Could not find include file: '" + file + "'");
        }
        addBlock(content);
        _includes.insert(file);
    }
}

const string& SourceCode::asString() const
{
    return _code;
}


void FragmentCompiler::compileFunctionCall(Context& context, const Fragment& frag, SourceCode& result)
{
    // Declare all outputs.
    for (size_t i = 0; i < frag.numOutputs(); ++i)
    {
        const Fragment::Output* output = frag.getOutput(i);
        declareVariable(context, *output, false, result);
    }

    result.beginLine();
    result.addString("void " + frag.getName().str() + "(");

    string delim = "";

    // Add all inputs
    for (size_t i = 0; i < frag.numInputs(); ++i)
    {
        const Fragment::Input* input = frag.getInput(i);
        result.addString(delim + input->variable.str());
        delim = ", ";
    }

    // Add all outputs
    for (size_t i = 0; i < frag.numOutputs(); ++i)
    {
        const Fragment::Output* output = frag.getOutput(i);
        result.addString(delim + output->variable.str());
        delim = ", ";
    }

    result.endLine();
}

void FragmentCompiler::compileFunction(Context& context, const Fragment& frag, SourceCode& result)
{
//    const Syntax& syntax = context.getCodeGenerator()->getSyntax();

    // Emit any include files.
    if (!frag.getIncludes().empty())
    {
        for (const string& file : frag.getIncludes())
        {
            string modifiedFile = file;
            tokenSubstitution(context.getSubstitutions(), modifiedFile);
            const FilePath resolvedFile = RtApi::get().getSearchPath().find(modifiedFile);
            result.addInclude(resolvedFile);
        }
        result.newLine();
    }

    // Add function definition.
    result.addBlock(frag.getSourceCode());
}

void FragmentCompiler::compileShader(Context& context, const Fragment& frag, SourceCode& result)
{
    compileFunction(context, frag, result);
}

void FragmentCompiler::declareVariable(Context& , const Fragment::Output& output, bool assignDefault, SourceCode& result)
{
    result.beginLine();
    result.addString(output.type.str() + " " + output.variable.str());
    if (assignDefault)
    {
    }
    result.endLine();
}

} // namepspace Codegen
} // namepspace MaterialX
