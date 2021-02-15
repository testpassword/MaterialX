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


FragmentCompiler::FragmentCompiler(const Context& context) :
    _context(context)
{
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
