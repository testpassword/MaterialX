//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/Fragment.h>
#include <MaterialXRuntime/Codegen/CodeGenerator.h>
#include <MaterialXRuntime/Codegen/CodegenContext.h>
#include <MaterialXRuntime/RtApi.h>

#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <MaterialXGenShader/Syntax.h>
#include <MaterialXGenShader/Util.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

Fragment::Fragment(const RtToken& name) :
    _name(name)
{
}

FragmentPtr Fragment::create(const RtToken& name)
{
    return std::make_shared<Fragment>(name);
}


Fragment::Input* Fragment::createInput(const RtToken& type, const RtToken& name)
{
    Input* input = new Input();
    input->parent = this;
    input->type = type;
    input->name = name;
    input->variable = name;
    input->value = RtValue::createNew(type, _allocator);
    input->connection = nullptr;
    _inputs.push_back(input);
    return input;
}

Fragment::Output* Fragment::createOutput(const RtToken& type, const RtToken& name)
{
    Output* output = new Output();
    output->parent = this;
    output->type = type;
    output->name = name;
    output->variable = name;
    _outputs.push_back(output);
    return output;
}

void Fragment::setSourceCode(const string& source)
{
    static const string INCLUDE = "#include";
    static const char QUOTE = '\"';

    std::stringstream stream(source);
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
                    _includes.insert(filename);
                }
            }
        }
        else
        {
            _sourceCode += line + '\n';
        }
    }
}


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
        throw ExceptionShaderGenError("End scope called with no scope active, please check your beginScope/endScope calls");
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
            throw ExceptionShaderGenError("Could not find include file: '" + file + "'");
        }
        addBlock(content);
        _includes.insert(file);
    }
}

const string& SourceCode::asString() const
{
    return _code;
}


void FragmentCompiler::compileFunctionCall(CodegenContext& context, const Fragment& frag, SourceCode& result)
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

void FragmentCompiler::compileFunction(CodegenContext& context, const Fragment& frag, SourceCode& result)
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

void FragmentCompiler::compileShader(CodegenContext& context, const Fragment& frag, SourceCode& result)
{
    compileFunction(context, frag, result);
}

void FragmentCompiler::declareVariable(CodegenContext& , const Fragment::Output& output, bool assignDefault, SourceCode& result)
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
