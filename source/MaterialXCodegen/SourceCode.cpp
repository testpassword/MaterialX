//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/SourceCode.h>

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

} // namespace Codegen
} // namespace MaterialX
