//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_SYNTAX_H
#define MATERIALX_CODEGEN_SYNTAX_H

/// @file
/// Base class for syntax handling for code generators

#include <MaterialXRuntime/Library.h>
#include <MaterialXRuntime/RtToken.h>
#include <MaterialXRuntime/RtValue.h>

namespace MaterialX
{
namespace Codegen
{

/// @class TypeSyntax
/// Base class for syntax handling of types.
class TypeSyntax
{
public:
    virtual ~TypeSyntax() { }

    /// Returns the type this syntax is for.
    const RtToken& getType() const { return _type; }

    /// Returns the type name.
    const string& getTypeName() const { return _typeName; }

    /// Returns a type alias if needed to define the type in the target language.
    const string& getTypeAlias() const { return _typeAlias; }

    /// Returns a type definition if needed to define the type in the target language.
    const string& getTypeDefinition() const { return _typeDefinition; }

    /// Returns the default value for this type.
    const string& getDefaultValue() const { return _defaultValue; }

    /// Returns the default value for this type in a shader interface context.
    const string& getInterfaceDefaultValue() const { return _interfaceDefaultValue; }

    /// Returns a value formatted according to this type syntax.
    virtual string getValue(const RtValue& value) const = 0;

    /// Returns a value formatted according to this type syntax
    /// when used in a shader interface context.
    virtual string getInterfaceValue(const RtValue& value) const { return getValue(value); }

    /// Returns the syntax for accessing type members if the type 
    /// can be swizzled.
    const StringVec& getMembers() const { return _members; }

protected:
    /// Protected constructor
    TypeSyntax(const RtToken& type, const string& name, const string& defaultValue, const string& uniformDefaultValue,
        const string& typeAlias, const string& typeDefinition, const StringVec& members);

    const RtToken _type;                 // type
    const string _typeName;              // type name
    const string _defaultValue;          // default value syntax
    const string _interfaceDefaultValue; // default value syntax when assigned to uniforms
    const string _typeAlias;             // type alias if needed in source code
    const string _typeDefinition;        // custom type definition if needed in source code
    const StringVec _members;            // syntax for member access

    static const StringVec EMPTY_MEMBERS;
};

/// Specialization of TypeSyntax for scalar types.
class ScalarTypeSyntax : public TypeSyntax
{
public:
    ScalarTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& uniformDefaultValue,
        const string& typeAlias = EMPTY_STRING, const string& typeDefinition = EMPTY_STRING);

    string getValue(const RtValue& value) const override;
};

/// Specialization of TypeSyntax for string types.
class StringTypeSyntax : public ScalarTypeSyntax
{
public:
    StringTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& uniformDefaultValue,
        const string& typeAlias = EMPTY_STRING, const string& typeDefinition = EMPTY_STRING);

    string getValue(const RtValue& value) const override;
};

/// Specialization of TypeSyntax for aggregate types.
class AggregateTypeSyntax : public TypeSyntax
{
public:
    AggregateTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& uniformDefaultValue,
        const string& typeAlias = EMPTY_STRING, const string& typeDefinition = EMPTY_STRING,
        const StringVec& members = EMPTY_MEMBERS);

    string getValue(const RtValue& value) const override;
};


/// @class Syntax
/// Base class for syntax handlers used by shader generators
class Syntax
{
  public:
    /// Punctuation types
    enum Punctuation
    {
        PARENTHESES,
        CURLY_BRACKETS,
        SQUARE_BRACKETS,
        DOUBLE_SQUARE_BRACKETS
    };

  public:
    virtual ~Syntax() { }

    /// Register syntax handling for a data type.
    /// Required to be set for all supported data types.
    void registerTypeSyntax(TypeSyntaxPtr syntax);

    /// Register names that are reserved words not to be used by a code generator when naming
    /// variables and functions. Keywords, types, built-in functions etc. should be 
    /// added to this set. Multiple calls will add to the internal set of names.
    void registerReservedWords(const StringSet& names);

    /// Register a set string replacements for disallowed tokens 
    /// for a code generator when naming variables and functions. 
    /// Multiple calls will add to the internal set of tokens.
    void registerInvalidTokens(const StringMap& tokens);

    /// Returns a set of names that are reserved words for this language syntax.
    const StringSet& getReservedWords() const { return _reservedWords; }

    /// Returns a mapping from disallowed tokens to replacement strings for this language syntax.
    const StringMap& getInvalidTokens() const { return _invalidTokens; }

    /// Returns the type syntax object for a named type.
    /// Throws an exception if a type syntax is not defined for the given type.
    const TypeSyntax& getTypeSyntax(const RtToken& type) const;

    /// Returns an array of all registered type syntax objects
    const vector<TypeSyntaxPtr>& getTypeSyntaxes() const { return _typeSyntaxes; }

    /// Returns the name syntax of the given type
    const string& getTypeName(const RtToken& type) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getTypeName();
    }

    /// Returns a type alias for the given data type.
    /// If not used returns an empty string.
    const string& getTypeAlias(const RtToken& type) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getTypeAlias();
    }

    /// Returns a custom type definition if needed for the given data type.
    /// If not used returns an empty string.
    const string& getTypeDefinition(const RtToken& type) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getTypeDefinition();
    }

    /// Returns the default value string for the given type
    const string& getDefaultValue(const RtToken& type) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getDefaultValue();
    }

    /// Returns the default value string for this type in a shader interface context.
    const string& getInterfaceDefaultValue(const RtToken& type) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getInterfaceDefaultValue();
    }

    /// Returns the value string for a given type and value object
    virtual string getValue(const RtToken& type, const RtValue& value) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getValue(value);
    }

    /// Returns the value string for a given type and value object
    /// in a shader interface context.
    virtual string getInterfaceValue(const RtToken& type, const RtValue& value) const
    {
        const TypeSyntax& syntax = getTypeSyntax(type);
        return syntax.getInterfaceValue(value);
    }

    /// Get syntax for a swizzled variable
//    virtual string getSwizzledVariable(const string& srcName, const RtToken& srcType, const string& channels, const RtToken& dstType) const;

    /// Get swizzled value
//    virtual RtValue getSwizzledValue(const RtValue& value, const RtToken& srcType, const string& channels, const RtToken& dstType) const;

    /// Returns a type qualifier to be used when declaring types for input variables.
    /// Default implementation returns empty string and derived syntax classes should
    /// override this method.
    virtual const string& getInputQualifier() const { return EMPTY_STRING; };

    /// Returns a type qualifier to be used when declaring types for output variables.
    /// Default implementation returns empty string and derived syntax classes should
    /// override this method.
    virtual const string& getOutputQualifier() const { return EMPTY_STRING; };

    /// Get the qualifier used when declaring constant variables.
    /// Derived classes must define this method. 
    virtual const string& getConstantQualifier() const = 0;

    /// Get the qualifier used when declaring uniform variables.
    /// Default implementation returns empty string and derived syntax classes should
    /// override this method.
    virtual const string& getUniformQualifier() const { return EMPTY_STRING; };

    /// Return the characters used for a newline.
    virtual const string& getNewline() const { return NEWLINE; };

    /// Return the characters used for a single indentation level.
    virtual const string& getIndentation() const { return INDENTATION; };

    /// Return the characters used to begin/end a string definition.
    virtual const string& getStringQuote() const { return STRING_QUOTE; };

    /// Return the string pattern used for a file include statement.
    virtual const string& getIncludeStatement() const { return INCLUDE_STATEMENT; };

    /// Return the characters used for single line comment.
    virtual const string& getSingleLineComment() const { return SINGLE_LINE_COMMENT; };

    /// Return the characters used to begin a multi line comments block.
    virtual const string& getBeginMultiLineComment() const { return BEGIN_MULTI_LINE_COMMENT; };

    /// Return the characters used to end a multi line comments block.
    virtual const string& getEndMultiLineComment() const { return END_MULTI_LINE_COMMENT; };

    /// Return the file extension used for source code files in this language.
    virtual const string& getSourceFileExtension() const = 0;

    /// Return the array suffix to use for declaring an array type.
//    virtual string getArrayTypeSuffix(const RtToken&, const RtValue&) const { return EMPTY_STRING; };

    /// Return the array suffix to use for declaring an array variable.
//    virtual string getArrayVariableSuffix(const RtToken& type, const RtValue& value) const;

    /// Query if given type is suppored in the syntax.
    /// By default all types are assumed to be supported.
//    virtual bool typeSupported(const TypeDesc* type) const;

    /// Modify the given name string to remove any invalid characters or tokens.
    virtual void makeValidName(string& name) const;

    /// Create a unique identifier from the given name.
    virtual RtToken createIdentifier(const string& name, RtTokenMap<size_t>& identifiers) const;

    /// Create a unique variable name for the given name and type.
    /// The method is used for naming variables (inputs and outputs) in generated code.
    /// Derived classes can override this method to have a custom naming strategy.
    /// Default implementation adds a number suffix, or increases an existing number suffix, 
    /// on the name string if there is a name collision.
    virtual RtToken createVariable(const string& name, const RtToken& type, RtTokenMap<size_t>& identifiers) const;

    /// Given an input specification attempt to remap this to an enumeration which is accepted by
    /// the shader generator. The enumeration may be converted to a different type than the input.
    /// @param value The value string to remap.
    /// @param type The type of the value to remap,
    /// @param enumNames Type enumeration names
    /// @param result Enumeration type and value (returned).
    /// @return Return true if the remapping was successful.
//    virtual bool remapEnumeration(const string& value, const TypeDesc* type, const string& enumNames,
//                                  std::pair<const TypeDesc*, ValuePtr>& result) const;

    /// Constants with commonly used strings.
    static const string NEWLINE;
    static const string SEMICOLON;
    static const string COMMA;

  protected:
    /// Protected constructor
    Syntax();

    vector<TypeSyntaxPtr> _typeSyntaxes;
    RtTokenMap<size_t> _typeSyntaxByType;

    StringSet _reservedWords;
    StringMap _invalidTokens;

    static const string INDENTATION;
    static const string STRING_QUOTE;
    static const string INCLUDE_STATEMENT;
    static const string SINGLE_LINE_COMMENT;
    static const string BEGIN_MULTI_LINE_COMMENT;
    static const string END_MULTI_LINE_COMMENT;

    static const std::unordered_map<char, size_t> CHANNELS_MAPPING;
};



} // namespace Codegen
} // namespace MaterialX

#endif
