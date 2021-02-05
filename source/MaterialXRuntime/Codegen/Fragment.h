//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_FRAGMENT_H
#define MATERIALX_CODEGEN_FRAGMENT_H

/// @file Fragment.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>
#include <MaterialXRuntime/RtValue.h>

#include <MaterialXRuntime/Codegen/Syntax.h>

namespace MaterialX
{
namespace Codegen
{

/// @class Fragment
/// Class holding a fragment of code.
class Fragment : public RtSharedBase<Fragment>
{
public:
    struct Output
    {
        Fragment* parent;
        RtToken type;
        RtToken name;
        RtToken variable;
    };

    struct Input : public Output
    {
        RtValue value;
        Output* connection;
    };

    /// Constructor.
    Fragment(const RtToken& name);

    /// Create a new fragment.
    static FragmentPtr create(const RtToken& name);

    /// Return the fragment name.
    const RtToken& getName() const
    {
        return _name;
    }

    Input* createInput(const RtToken& type, const RtToken& name);
    Output* createOutput(const RtToken& type, const RtToken& name);

    size_t numInputs() const
    {
        return _inputs.size();
    }
    Input* getInput(size_t index) const
    {
        return _inputs[index];
    }

    size_t numOutputs() const
    {
        return _inputs.size();
    }
    Output* getOutput(size_t index) const
    {
        return _outputs[index];
    }

    /// Set fragment source code.
    void setSourceCode(const string& source);

    /// Return fragment source code.
    const string& getSourceCode() const
    {
        return _sourceCode;
    }

    /// Return include file dependencies.
    const StringSet& getIncludes() const
    {
        return _includes;
    }

protected:
    /// Name of the fragment.
    const RtToken _name;

    /// Source code for this fragment.
    string _sourceCode;

    /// Set of include files that has been included.
    StringSet _includes;

    /// Allocator for large values.
    RtAllocator _allocator;

    vector<Input*> _inputs;
    vector<Output*> _outputs;
};


class SourceCode
{
public:
    /// Contructor.
    SourceCode(const Syntax& syntax);

    /// Start a new scope using the given bracket type.
    void beginScope(Syntax::Punctuation punc = Syntax::CURLY_BRACKETS);

    /// End the current scope.
    void endScope(bool semicolon = false, bool newline = true);

    /// Start a new line.
    void beginLine();

    /// End the current line.
    void endLine(bool semicolon = true);

    /// Add a newline character.
    void newLine();

    /// Add a generic code string.
    void addString(const string& str);

    /// Add a line of code, optionally appending a semicolon.
    void addLine(const string& str, bool semicolon = true);

    /// Add a block of code.
    void addBlock(const string& str);

    /// Add a single line code comment.
    void addComment(const string& str);

    /// Add an include file.
    void addInclude(const string& file);

    /// Return the resulting source code.
    const string& asString() const;

private:
    const Syntax& _syntax;
    int _indentations;
    vector<Syntax::Punctuation> _scopes;
    StringSet _includes;
    string _code;
};



class FragmentCompiler;
using FragmentCompilerPtr = RtSharedPtr<FragmentCompiler>;

/// @class FragmentCompiler
/// Class compiling fragments into functions and shaders.
class FragmentCompiler : public RtSharedBase<FragmentCompiler>
{
public:
    virtual void compileFunction(CodegenContext& context, const Fragment& frag, SourceCode& result);
    virtual void compileFunctionCall(CodegenContext& context, const Fragment& frag, SourceCode& result);
    virtual void compileShader(CodegenContext& context, const Fragment& frag, SourceCode& result);

protected:
    void declareVariable(CodegenContext& context, const Fragment::Output& output, bool assignDefault, SourceCode& result);
};

} // namepspace Codegen
} // namepspace MaterialX

#endif
