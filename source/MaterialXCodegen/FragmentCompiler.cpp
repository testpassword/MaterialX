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

void SourceCode::setIncluded(const RtToken& file)
{
    _includes.insert(file);
}

bool SourceCode::isIncluded(const RtToken& file)
{
    return _includes.count(file) > 0;
}

const string& SourceCode::asString() const
{
    return _code;
}


void FragmentCompiler::compileFunctionCall(const Fragment& frag, SourceCode& result)
{
    // Declare all outputs.
    for (size_t i = 0; i < frag.numOutputs(); ++i)
    {
        const Fragment::Output* output = frag.getOutput(i);
        declareVariable(*output, true, result);
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
    result.addString(")");
    result.endLine();
}

FragmentCompiler::FragmentCompiler(Context& context) :
    _context(context),
    _syntax(context.getSyntax())
{
}

void FragmentCompiler::compileFunction(const Fragment& frag, SourceCode& result)
{
    // Emit any include files.
    if (!frag.getIncludes().empty())
    {
        for (const RtToken& file : frag.getIncludes())
        {
            string modifiedFile = file.str();
            tokenSubstitution(_context.getSubstitutions(), modifiedFile);
            const FilePath resolvedFile = RtApi::get().getSearchPath().find(modifiedFile);
            emitInclude(resolvedFile, result);
        }
        result.newLine();
    }

    // Add function definition.
    emitBlock(frag.getSourceCode(), result);
}

void FragmentCompiler::compileShader(const Fragment& frag, SourceCode& result)
{
    compileFunction(frag, result);
}

void FragmentCompiler::declareVariable(const Fragment::Output& output, bool assignDefault, SourceCode& result)
{
    result.beginLine();
    result.addString(_syntax.getTypeName(output.type) + " " + output.variable.str());
    if (assignDefault)
    {
        result.addString(" = " + _syntax.getDefaultValue(output.type));
    }
    result.endLine();
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
        const string content = readFile(file);
        if (content.empty())
        {
            throw ExceptionRuntimeError("Could not find include file: '" + f.str() + "'");
        }
        emitBlock(content, result);
        result.setIncluded(f);
    }
}

} // namespace Codegen
} // namespace MaterialX
