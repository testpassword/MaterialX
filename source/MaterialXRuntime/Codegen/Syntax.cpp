//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Codegen/Syntax.h>

namespace MaterialX
{
namespace Codegen
{

const string Syntax::NEWLINE = "\n";
const string Syntax::SEMICOLON = ";";
const string Syntax::COMMA = ",";
const string Syntax::INDENTATION = "    ";
const string Syntax::STRING_QUOTE = "\"";
const string Syntax::INCLUDE_STATEMENT = "#include";
const string Syntax::SINGLE_LINE_COMMENT = "// ";
const string Syntax::BEGIN_MULTI_LINE_COMMENT = "/* ";
const string Syntax::END_MULTI_LINE_COMMENT = " */";

const std::unordered_map<char, size_t> Syntax::CHANNELS_MAPPING =
{
    { 'r', 0 }, { 'x', 0 },
    { 'g', 1 }, { 'y', 1 },
    { 'b', 2 }, { 'z', 2 },
    { 'a', 3 }, { 'w', 3 }
};

Syntax::Syntax()
{
}

void Syntax::registerTypeSyntax(TypeSyntaxPtr syntax)
{
    auto it = _typeSyntaxByType.find(syntax->getType());
    if (it != _typeSyntaxByType.end())
    {
        _typeSyntaxes[it->second] = syntax;
    }
    else
    {
        _typeSyntaxes.push_back(syntax);
        _typeSyntaxByType[syntax->getType()] = _typeSyntaxes.size() - 1;
    }

    // Make this type name a restricted name
    registerReservedWords({ syntax->getTypeName() });
}

void Syntax::registerReservedWords(const StringSet& names)
{
    _reservedWords.insert(names.begin(), names.end());
}

void Syntax::registerInvalidTokens(const StringMap& tokens)
{
    _invalidTokens.insert(tokens.begin(), tokens.end());
}

/// Returns the type syntax object for a named type.
/// Throws an exception if a type syntax is not defined for the given type.
const TypeSyntax& Syntax::getTypeSyntax(const RtToken& type) const
{
    auto it = _typeSyntaxByType.find(type);
    if (it == _typeSyntaxByType.end())
    {
        throw ExceptionRuntimeError("No syntax is defined for the given type '" + type.str() + "'.");
    }
    return *_typeSyntaxes[it->second];
}

/*
string Syntax::getSwizzledVariable(const string& srcName, const RtToken& srcType, const string& channels, const RtToken& dstType) const
{
    const TypeSyntax& srcSyntax = getTypeSyntax(srcType);
    const TypeSyntax& dstSyntax = getTypeSyntax(dstType);

    const StringVec& srcMembers = srcSyntax.getMembers();

    StringVec membersSwizzled;

    for (size_t i = 0; i < channels.size(); ++i)
    {
        const char ch = channels[i];
        if (ch == '0' || ch == '1')
        {
            membersSwizzled.push_back(string(1,ch));
            continue;
        }

        auto it = CHANNELS_MAPPING.find(ch);
        if (it == CHANNELS_MAPPING.end())
        {
            throw ExceptionRuntimeError("Invalid channel pattern '" + channels + "'.");
        }

        if (srcMembers.empty())
        {
            membersSwizzled.push_back(srcName);
        }
        else
        {
            int channelIndex = srcType->getChannelIndex(ch);
            if (channelIndex < 0 || channelIndex >= static_cast<int>(srcMembers.size()))
            {
                throw ExceptionRuntimeError("Given channel index: '" + string(1,ch) + "' in channels pattern is incorrect for type '" + srcType->getName() + "'.");
            }
            membersSwizzled.push_back(srcName + srcMembers[channelIndex]);
        }
    }

    return dstSyntax.getValue(membersSwizzled, false);
}

ValuePtr Syntax::getSwizzledValue(ValuePtr value, const RtToken& srcType, const string& channels, const RtToken& dstType) const
{
    const TypeSyntax& srcSyntax = getTypeSyntax(srcType);
    const vector<string>& srcMembers = srcSyntax.getMembers();

    StringStream ss;
    string delimiter = ", ";

    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (i == channels.size() - 1)
        {
            delimiter = "";
        }

        const char ch = channels[i];
        if (ch == '0' || ch == '1')
        {
            ss << string(1, ch);
            continue;
        }

        auto it = CHANNELS_MAPPING.find(ch);
        if (it == CHANNELS_MAPPING.end())
        {
            throw ExceptionRuntimeError("Invalid channel pattern '" + channels + "'.");
        }

        if (srcMembers.empty())
        {
            ss << value->getValueString();
        }
        else
        {
            int channelIndex = srcType->getChannelIndex(ch);
            if (channelIndex < 0 || channelIndex >= static_cast<int>(srcMembers.size()))
            {
                throw ExceptionRuntimeError("Given channel index: '" + string(1, ch) + "' in channels pattern is incorrect for type '" + srcType->getName() + "'.");
            }
            if (srcType == Type::FLOAT)
            {
                float v = value->asA<float>();
                ss << std::to_string(v);
            }
            else if (srcType == Type::INTEGER)
            {
                int v = value->asA<int>();
                ss << std::to_string(v);
            }
            else if (srcType == Type::BOOLEAN)
            {
                bool v = value->asA<bool>();
                ss << std::to_string(v);
            }
            else if (srcType == Type::COLOR3)
            {
                Color3 v = value->asA<Color3>();
                ss << std::to_string(v[channelIndex]);
            }
            else if (srcType == Type::COLOR4)
            {
                Color4 v = value->asA<Color4>();
                ss << std::to_string(v[channelIndex]);
            }
            else if (srcType == Type::VECTOR2)
            {
                Vector2 v = value->asA<Vector2>();
                ss << std::to_string(v[channelIndex]);
            }
            else if (srcType == Type::VECTOR3)
            {
                Vector3 v = value->asA<Vector3>();
                ss << std::to_string(v[channelIndex]);
            }
            else if (srcType == Type::VECTOR4)
            {
                Vector4 v = value->asA<Vector4>();
                ss << std::to_string(v[channelIndex]);
            }
        }
        ss << delimiter;
    }

    return Value::createValueFromStrings(ss.str(), getTypeName(dstType));
}

bool Syntax::typeSupported(const RtToken&) const
{
    return true;
}

string Syntax::getArrayVariableSuffix(const RtToken& type, const Value& value) const
{
    if (type->isArray())
    {
        if (value.isA<vector<float>>())
        {
            const size_t size = value.asA<vector<float>>().size();
            return "[" + std::to_string(size) + "]";
        }
        else if (value.isA<vector<int>>())
        {
            const size_t size = value.asA<vector<int>>().size();
            return "[" + std::to_string(size) + "]";
        }
    }
    return string();
}
*/

static bool isInvalidChar(char c)
{
    return !isalnum(c) && c != '_';
}

void Syntax::makeValidName(string& name) const
{
    std::replace_if(name.begin(), name.end(), isInvalidChar, '_');
    name = replaceSubstrings(name, _invalidTokens);
}

RtToken Syntax::createIdentifier(const string& name, RtTokenMap<size_t>& identifiers) const
{
    string validName(name);
    makeValidName(validName);

    RtToken id(validName);

    auto it = identifiers.find(id);
    if (it != identifiers.end())
    {
        // Name is not unique so append the counter and keep
        // incrementing until a unique name is found.
        string name2;
        do {
            name2 = validName + std::to_string(it->second++);
            id = RtToken(name2);
        } while (identifiers.count(id));
    }

    // Save it among the known identifiers.
    identifiers[id] = 1;

    return id;
}

RtToken Syntax::createVariable(const string& name, const RtToken& /*type*/, RtTokenMap<size_t>& identifiers) const
{
    // Default implementation just create an identifier fomr the name, 
    // but derived classes can override this for custom variable naming.
    return createIdentifier(name, identifiers);
}

/*
bool Syntax::remapEnumeration(const string&, const RtToken&, const string&, std::pair<const RtToken&, ValuePtr>&) const
{
    return false;
}
*/

const StringVec TypeSyntax::EMPTY_MEMBERS;

TypeSyntax::TypeSyntax(const RtToken& type, const string& typeName, 
                       const string& defaultValue, const string& interfaceDefaultValue,
                       const string& typeAlias, const string& typeDefinition, 
                       const StringVec& members) :
    _type(type),
    _typeName(typeName),
    _defaultValue(defaultValue),
    _interfaceDefaultValue(interfaceDefaultValue),
    _typeAlias(typeAlias),
    _typeDefinition(typeDefinition),
    _members(members)
{
}


ScalarTypeSyntax::ScalarTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& interfaceDefaultValue,
                                   const string& typeAlias, const string& typeDefinition) :
    TypeSyntax(type, typeName, defaultValue, interfaceDefaultValue, typeAlias, typeDefinition, EMPTY_MEMBERS)
{
}

string ScalarTypeSyntax::getValue(const RtValue& value) const
{
    string result;
    RtValue::toString(_type, value, result);
    return result;
}


StringTypeSyntax::StringTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& interfaceDefaultValue,
                                   const string& typeAlias, const string& typeDefinition) :
    ScalarTypeSyntax(type, typeName, defaultValue, interfaceDefaultValue, typeAlias, typeDefinition)
{
}

string StringTypeSyntax::getValue(const RtValue& value) const
{
    string result;
    RtValue::toString(_type, value, result);
    return "\"" + result + "\"";
}


AggregateTypeSyntax::AggregateTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& interfaceDefaultValue,
                                         const string& typeAlias, const string& typeDefinition, const StringVec& members) :
    TypeSyntax(type, typeName, defaultValue, interfaceDefaultValue, typeAlias, typeDefinition, members)
{
}

string AggregateTypeSyntax::getValue(const RtValue& value) const
{
    string result;
    RtValue::toString(_type, value, result);
    return getTypeName() + "(" + result + ")";
}

} // namespace Codegen
} // namespace MaterialX
