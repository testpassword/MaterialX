//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTCODEGENERATOR_H
#define MATERIALX_RTCODEGENERATOR_H

/// @file RtCodeGenerator.h
/// TODO: Docs

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>

#include <MaterialXGenShader/Syntax.h>

namespace MaterialX
{

/// @class RtFragment
/// Class holding a fragment of code produced by code generation.
class RtFragment : public RtSharedBase<RtFragment>
{
public:
    /// Contructor.
    RtFragment(const RtToken& name, ConstSyntaxPtr syntax, RtCodegenContextPtr context);

    /// Create a new fragment.
    static RtFragmentPtr create(const RtToken& name, ConstSyntaxPtr syntax, RtCodegenContextPtr context);

    /// Return the fragment name.
    const RtToken& getName() const;

    /// Return the syntax object.
    const Syntax& getSyntax() const;

    /// Return the fragment source code.
    const string& getSourceCode() const;

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

    /// Add the contents of an include file. Making sure it is 
    /// only included once for this fragment.
    void addIncludeFile(const string& file);

    /// Replace tokens with identifiers according to the given substitutions map.
    void replaceTokens(const StringMap& substitutions);

protected:
    /// Name of the fragment
    const RtToken _name;

    /// Syntax for the language used.
    ConstSyntaxPtr _syntax;

    /// Codegen context data.
    RtCodegenContextPtr _context;

    /// Current indentation level.
    int _indentations;

    /// Current scope.
    vector<Syntax::Punctuation> _scopes;

    /// Set of include files that has been included.
    StringSet _includes;

    /// Set of hash ID's for functions that has been defined.
    std::set<size_t> _functions;

    /// Resulting source code for this fragment.
    string _code;
};


/// @class RtCodegenResult
/// Class holding the result returned by code generation.
class RtCodegenResult : public RtSharedBase<RtCodegenResult>
{
public:
    /// Create a new instance of this class.
    static RtCodegenResultPtr create();

    /// Add a new fragment to the results.
    void addFragment(const RtFragmentPtr& fragment);

    /// Remove a fragment from the results.
    void removeFragment(const RtToken& name);

    /// Return the number of fragments contained in this result.
    size_t numFragments() const;

    /// Return a fragment by index.
    RtFragmentPtr getFragment(size_t index) const;

    /// Return a fragment by name.
    RtFragmentPtr getFragment(const RtToken& name) const;

protected:
    vector<RtFragmentPtr> _fragmentsOrder;
    RtTokenMap<RtFragmentPtr> _fragments;
};


/// @class RtCodeGenerator
/// Abstract base class for code generators.
class RtCodeGenerator : public RtSharedBase<RtCodeGenerator>
{
public:
    /// Destructor.
    virtual ~RtCodeGenerator() {}

    /// Return the target identifier for this generator.
    virtual const RtToken& getTarget() const = 0;

    /// Create a new options instance for this generator.
    virtual RtCodegenOptionsPtr createOptions() const;

    /// Create a new context instance for this generator.
    virtual RtCodegenContextPtr createContext(RtCodegenOptionsPtr options) const = 0;

    /// Generate code for the given prim and path.
    virtual RtCodegenResultPtr generate(const RtPrim& prim, const RtPath& path, RtCodegenContextPtr context) const = 0;

    /// Return the implementation for the given nodedef.
    /// Throws an exception if no valid codegen implementaion is found.
    virtual RtCodegenImpl getCodegenImpl(const RtNodeDef& nodedef) const;

protected:
    /// Constructor.
    RtCodeGenerator() {}
};


/// @class RtOslGenerator
/// Code generator for vanilla OSL.
class RtOslGenerator : public RtCodeGenerator
{
public:
    /// Create a new instance of this code generator.
    static RtCodeGeneratorPtr create();

    /// Return the target identifier for this generator.
    const RtToken& getTarget() const override;

    /// Create a new context instance for this generator.
    RtCodegenContextPtr createContext(RtCodegenOptionsPtr options) const override;

    /// Generate code for the given prim and path.
    RtCodegenResultPtr generate(const RtPrim& prim, const RtPath& path, RtCodegenContextPtr context) const override;

    /// Target identifier constant for this generator.
    static const RtToken TARGET;

protected:
    /// Constructor.
    RtOslGenerator();

    SyntaxPtr _syntax;
};

}

#endif