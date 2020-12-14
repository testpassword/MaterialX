//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_RTCODEGENERATOR_H
#define MATERIALX_RTCODEGENERATOR_H

/// @file RtCodeGenerator.h
/// TODO: Docs

#include <MaterialXRuntime/RtSchema.h>

#include <MaterialXGenShader/Syntax.h>

namespace MaterialX
{

class RtFragment;
class RtCodegenContext;
class RtCodegenResult;

using RtFragmentPtr = shared_ptr<RtFragment>;
using RtCodegenResultPtr = shared_ptr<RtCodegenResult>;
using RtCodegenContextPtr = shared_ptr<RtCodegenContext>;

/// @class RtCodegenOptions
/// Base class for options used by a code generator.
struct RtCodegenOptions
{
};

using RtCodegenOptionsPtr = shared_ptr<RtCodegenOptions>;

/// @class RtCodegenContext
/// Base class for context data needed by code generators.
/// Derived code generators can derive from this class to hold
/// custom context data and needed by the generator.
class RtCodegenContext
{
  public:
    /// Constructor.
    RtCodegenContext(RtCodegenOptionsPtr options) :
      _options(options)
    {}

    /// Destructor.
    virtual ~RtCodegenContext() {}

    /// Return the options struct.
    const RtCodegenOptions* getOptions() const
    {
        return _options.get();
    }

    /// Return the options struct.
    RtCodegenOptions* getOptions()
    {
        return _options.get();
    }

    /// Add reserved words that should not be used as
    /// identifiers during code generation.
    void addReservedWords(const StringSet& names)
    {
        _reservedWords.insert(names.begin(), names.end());
    }

    /// Return the set of reserved words that should not be used
    /// as identifiers during code generation.
    const StringSet& getReservedWords() const
    {
        return _reservedWords;
    }

  protected:
    // Options.
    RtCodegenOptionsPtr _options;

    // Set of globally reserved words.
    StringSet _reservedWords;
};


/// @class RtFragment
/// Class holding a fragment of code produced by code generation.
class RtFragment
{
public:
    /// Contructor.
    RtFragment(const RtToken& name, ConstSyntaxPtr syntax);

    /// Create a new fragment.
    static RtFragmentPtr create(const RtToken& name, ConstSyntaxPtr syntax);

    /// Return the fragment name.
    const RtToken& getName() const;

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

protected:
    /// Name of the fragment
    const RtToken _name;

    /// Syntax for the type of shader to generate.
    ConstSyntaxPtr _syntax;

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
class RtCodegenResult
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

using RtCodegenResultPtr = shared_ptr<RtCodegenResult>;


/// @class RtCodeGenerator
/// Abstract base class for code generators.
/// API schema for generating code from a prim.
class RtCodeGenerator : public RtSchemaBase
{
public:
    /// Destructor.
    virtual ~RtCodeGenerator() {}

    /// Return true if the given prim is compatible with this schema.
    bool isCompatible(const RtPrim& prim) const override;

    /// Return the target identifier for this generator.
    virtual const RtToken& getTarget() const = 0;

    /// Create a new options instance for this generator.
    virtual RtCodegenOptionsPtr createOptions() const;

    /// Create a new context instance for this generator.
    virtual RtCodegenContextPtr createContext() const;

    /// Generate code for the object pointed to by the given path
    /// on the prim attached to the schema.
    virtual RtCodegenResultPtr generate(const RtPath& path, RtCodegenContext& context) const = 0;

protected:
      /// Constructor attaching this API to a prim.
    explicit RtCodeGenerator(const RtPrim& prim) :
        RtSchemaBase(prim)
    {}
};


/// @class RtOslGenerator
/// Code generator for vanilla OSL.
class RtOslGenerator : public RtCodeGenerator
{
public:
    /// Constructor attaching this API to a prim.
    explicit RtOslGenerator(const RtPrim& prim);

    /// Return the target identifier for this generator.
    const RtToken& getTarget() const override;

    /// Generate code for the object pointed to by the given path
    /// on the prim attached to the schema.
    RtCodegenResultPtr generate(const RtPath& path, RtCodegenContext& context) const override;

protected:
    SyntaxPtr _syntax;
};

}

#endif
