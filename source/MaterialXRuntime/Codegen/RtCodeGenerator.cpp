//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>
#include <MaterialXRuntime/Codegen/RtOslSyntax.h>
#include <MaterialXRuntime/Codegen/RtGraphImpl.h>

#include <MaterialXRuntime/Private/Codegen/PvtGraphImpl.h>

#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <sstream>

namespace MaterialX
{

RtFragment::RtFragment(const RtToken& name, ConstSyntaxPtr syntax) :
    _name(name),
    _syntax(syntax),
    _indentations(0)
{
}

RtFragmentPtr RtFragment::create(const RtToken& name, ConstSyntaxPtr syntax)
{
    return std::make_shared<RtFragment>(name, syntax);
}

const RtToken& RtFragment::getName() const
{
    return _name;
}

const string& RtFragment::getSourceCode() const
{
    return _code;
}

void RtFragment::beginScope(Syntax::Punctuation punc)
{
    switch (punc) {
    case Syntax::CURLY_BRACKETS:
        beginLine();
        _code += "{" + _syntax->getNewline();
        break;
    case Syntax::PARENTHESES:
        beginLine();
        _code += "(" + _syntax->getNewline();
        break;
    case Syntax::SQUARE_BRACKETS:
        beginLine();
        _code += "[" + _syntax->getNewline();
        break;
    case Syntax::DOUBLE_SQUARE_BRACKETS:
        beginLine();
        _code += "[[" + _syntax->getNewline();
        break;
    }

    ++_indentations;
    _scopes.push_back(punc);
}

void RtFragment::endScope(bool semicolon, bool newline)
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
        _code += _syntax->getNewline();
}

void RtFragment::beginLine()
{
    for (int i = 0; i < _indentations; ++i)
    {
        _code += _syntax->getIndentation();
    }
}

void RtFragment::endLine(bool semicolon)
{
    if (semicolon)
    {
        _code += ";";
    }
    newLine();
}

void RtFragment::newLine()
{
    _code += _syntax->getNewline();
}

void RtFragment::addString(const string& str)
{
    _code += str;
}

void RtFragment::addLine(const string& str, bool semicolon)
{
    beginLine();
    addString(str);
    endLine(semicolon);
}

void RtFragment::addComment(const string& str)
{
    beginLine();
    _code += _syntax->getSingleLineComment() + str;
    endLine(false);
}

void RtFragment::addBlock(const string& str)
{
    const string& INCLUDE = _syntax->getIncludeStatement();
    const string& QUOTE = _syntax->getStringQuote();

    // Add each line in the block seperatelly
    // to get correct indentation
    std::stringstream stream(str);
    for (string line; std::getline(stream, line); )
    {
        size_t pos = line.find(INCLUDE);
        if (pos != string::npos)
        {
            size_t startQuote = line.find_first_of(QUOTE);
            size_t endQuote = line.find_last_of(QUOTE);
            if (startQuote != string::npos && endQuote != string::npos && endQuote > startQuote)
            {
                size_t length = (endQuote - startQuote) - 1;
                if (length)
                {
                    const string filename = line.substr(startQuote + 1, length);
                    addIncludeFile(filename);
                }
            }
        }
        else
        {
            addLine(line, false);
        }
    }
}

void RtFragment::addIncludeFile(const string& file)
{
    if (!_includes.count(file))
    {
        string content = readFile(file);
        if (content.empty())
        {
            throw ExceptionShaderGenError("Could not find include file: '" + file + "'");
        }
        _includes.insert(file);
        addBlock(content);
    }
}


RtCodegenResultPtr RtCodegenResult::create()
{
    return std::make_shared<RtCodegenResult>();
}

void RtCodegenResult::addFragment(const RtFragmentPtr& fragment)
{
    if (getFragment(fragment->getName()))
    {
        throw ExceptionShaderGenError("A fragment named '" + fragment->getName().str() + "' already exists");
    }

    _fragments[fragment->getName()] = fragment;
    _fragmentsOrder.push_back(fragment);
}

void RtCodegenResult::removeFragment(const RtToken& name)
{
    auto it = _fragments.find(name);
    if (it != _fragments.end())
    {
        _fragmentsOrder.erase(std::find(_fragmentsOrder.begin(), _fragmentsOrder.end(), it->second));
        _fragments.erase(it);
    }
}

size_t RtCodegenResult::numFragments() const
{
    return _fragmentsOrder.size();
}

RtFragmentPtr RtCodegenResult::getFragment(size_t index) const
{
    return _fragmentsOrder[index];
}

RtFragmentPtr RtCodegenResult::getFragment(const RtToken& name) const
{
    auto it = _fragments.find(name);
    return it != _fragments.end() ? it->second : nullptr;
}


bool RtCodeGenerator::isCompatible(const RtPrim& prim) const
{
    return prim && (prim.hasApi<RtNode>() || prim.hasApi<RtNodeGraph>());
}

RtCodegenOptionsPtr RtCodeGenerator::createOptions() const
{
    return std::make_shared<RtCodegenOptions>();
}

RtCodegenContextPtr RtCodeGenerator::createContext() const
{
    RtCodegenOptionsPtr options = createOptions();
    return std::make_shared<RtCodegenContext>(options);
}


RtOslGenerator::RtOslGenerator(const RtPrim& prim) :
    RtCodeGenerator(prim),
    _syntax(RtOslSyntax::create())
{
}

const RtToken& RtOslGenerator::getTarget() const
{
    static const RtToken GENOSL("genosl");
    return GENOSL;
}

RtCodegenResultPtr RtOslGenerator::generate(const RtPath& /*path*/, RtCodegenContext& /*context*/) const
{
    RtPrim prim = getPrim();
    if (!prim)
    {
        throw ExceptionRuntimeError("Prim attached to RtOslGenerator API is not valid");
    }

    PvtDataHandle implH;
    if (prim.hasApi<RtNodeGraph>())
    {
        implH = PvtGraphImpl::createNew(RtNodeGraph(prim));
    }
    else if (prim.hasApi<RtNode>())
    {
        implH = PvtGraphImpl::createNew(RtNode(prim));
    }
    else
    {
        throw ExceptionRuntimeError("Unsupported prim attached to RtOslGenerator API");
    }

    RtFragmentPtr frag = RtFragment::create(prim.getName(), _syntax);

    PvtGraphImpl* impl = implH->asA<PvtGraphImpl>();
    for (const PvtPrim* node : impl->getNodes())
    {
        frag->addLine(node->getName().str());
    }

    RtCodegenResultPtr result = RtCodegenResult::create();
    result->addFragment(frag);

    return result;
}

}
