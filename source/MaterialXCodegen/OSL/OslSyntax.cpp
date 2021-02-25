//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXCodegen/OSL/OslSyntax.h>

#include <MaterialXRuntime/RtTypeDef.h>
#include <MaterialXRuntime/RtApi.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

namespace
{

class OslBooleanTypeSyntax : public ScalarTypeSyntax
{
public:
    OslBooleanTypeSyntax() :
        ScalarTypeSyntax(RtType::BOOLEAN, "int", "0", "0")
    {}

    string getValue(const RtValue& value) const override
    {
        return value.asBool() ? "1" : "0";
    }

    string getValue(const StringVec& values) const override
    {
        return values.size() && values[0] == "true" ? "1" : "0";
    }
};

// In OSL vector2, vector4 and color4 are custom struct types and require a different
// value syntax for shader interface contexts. So override the aggregate type syntax to support this.
class OslStructTypeSyntax : public AggregateTypeSyntax
{
  public:
    OslStructTypeSyntax(const RtToken& type, const string& typeName, const string& defaultValue, const string& interfaceDefaultValue,
                        const string& typeAlias = EMPTY_STRING, const string& typeDefinition = EMPTY_STRING,
                        const StringVec& members = EMPTY_MEMBERS) :
        AggregateTypeSyntax(type, typeName, defaultValue, interfaceDefaultValue, typeAlias, typeDefinition, members)
    {}

    string getInterfaceValue(const RtValue& value) const override
    {
        string result;
        RtValue::toString(_type, value, result);
        return "{" + result + "}";
    }

    string getValue(const StringVec& values) const override
    {
        return getValue(values, false);
    }

    string getInterfaceValue(const StringVec& values) const override
    {
        return getValue(values, true);
    }

private:
    string getValue(const StringVec& values, bool curlyBrackets) const
    {
        if (values.empty())
        {
            throw ExceptionRuntimeError("No values given to construct a value");
        }
        std::stringstream ss;
        ss << (curlyBrackets ? "{" : getTypeName() + "(" ) << values[0];
        for (size_t i = 1; i < values.size(); ++i)
        {
            ss << ", " << values[i];
        }
        ss << (curlyBrackets ? "}" : ")");
        return ss.str();
    }

};

// For the color4 type we need even more specialization since it's a struct of a struct:
//
// struct color4 {
//    color rgb;
//    float a;
// }
//
class OslColor4TypeSyntax : public OslStructTypeSyntax
{
  public:
    OslColor4TypeSyntax() :
        OslStructTypeSyntax(RtType::COLOR4, "color4", "color4(color(0.0), 0.0)", "{color(0.0), 0.0}", EMPTY_STRING, EMPTY_STRING, OslSyntax::COLOR4_MEMBERS)
    {}

    string getValue(const RtValue& value) const override
    {
        std::stringstream ss;
        RtApi::get().setStreamFloatFormat(ss);
        const Color4& c = value.asColor4();
        ss << "color4(color(" << c[0] << ", " << c[1] << ", " << c[2] << "), " << c[3] << ")";
        return ss.str();
    }

    string getInterfaceValue(const RtValue& value) const override
    {
        std::stringstream ss;
        RtApi::get().setStreamFloatFormat(ss);
        const Color4& c = value.asColor4();
        ss << "{color(" << c[0] << ", " << c[1] << ", " << c[2] << "), " << c[3] << "}";
        return ss.str();
    }

    string getValue(const StringVec& values) const override
    {
        if (values.size() < 4)
        {
            throw ExceptionRuntimeError("Too few values given to construct a color4 value");
        }
        std::stringstream ss;
        ss << "color4(color(" << values[0] << ", " << values[1] << ", " << values[2] << "), " << values[3] << ")";
        return ss.str();
    }

    string getInterfaceValue(const StringVec& values) const override
    {
        if (values.size() < 4)
        {
            throw ExceptionRuntimeError("Too few values given to construct a color4 value");
        }
        std::stringstream ss;
        ss << "{color(" << values[0] << ", " << values[1] << ", " << values[2] << "), " << values[3] << "}";
        return ss.str();
    }
};

class OSLMatrix3TypeSyntax : public AggregateTypeSyntax
{
  public:
    OSLMatrix3TypeSyntax(const string& name, const string& defaultValue, const string& interfaceDefaultValue,
                         const string& typeAlias = EMPTY_STRING, const string& typeDefinition = EMPTY_STRING,
                         const StringVec& members = EMPTY_MEMBERS) :
        AggregateTypeSyntax(RtType::MATRIX33, name, defaultValue, interfaceDefaultValue, typeAlias, typeDefinition, members)
    {}

    string getValue(const RtValue& value) const override
    {
        std::stringstream ss;
        RtApi::get().setStreamFloatFormat(FloatFormat::FIXED, 3, ss);

        string result;
        RtValue::toString(_type, value, result);

        StringVec values = splitString(result, ",");

        ss << getTypeName() << "(";
        for (size_t i = 0; i<values.size(); i++)
        {
            ss << values[i] << ", ";
            if ((i+1) % 3 == 0)
            {
                ss << "0.000" << ", ";
            }
        }
        static string ROW_4("0.000, 0.000, 0.000, 1.000");
        ss << ROW_4 << ")";

        return ss.str();
    }
};

} // anonymous namespace

const string OslSyntax::OUTPUT_QUALIFIER = "output ";
const string OslSyntax::SOURCE_FILE_EXTENSION = ".osl";
const StringVec OslSyntax::VECTOR_MEMBERS  = { "[0]", "[1]", "[2]" };
const StringVec OslSyntax::VECTOR2_MEMBERS = { ".x", ".y" };
const StringVec OslSyntax::VECTOR4_MEMBERS = { ".x", ".y", ".z", ".w" };
const StringVec OslSyntax::COLOR2_MEMBERS  = { ".r", ".a" };
const StringVec OslSyntax::COLOR4_MEMBERS  = { ".rgb[0]", ".rgb[1]", ".rgb[2]", ".a" };

//
// OslSyntax methods
//

OslSyntax::OslSyntax()
{
    // Add in all reserved words and keywords in OSL
    registerReservedWords(
    {
        "and", "break", "closure", "color", "continue", "do", "else", "emit", "float", "for", "if", "illuminance",
        "illuminate", "int", "matrix", "normal", "not", "or", "output", "point", "public", "return", "string",
        "struct", "vector", "void", "while",
        "bool", "case", "catch", "char", "class", "const", "delete", "default", "double", "enum", "extern",
        "false", "friend", "goto", "inline", "long", "new", "operator", "private", "protected", "short",
        "signed", "sizeof", "static", "switch", "template", "this", "throw", "true", "try", "typedef", "uniform",
        "union", "unsigned", "varying", "virtual", "volatile",
        "emission", "background", "diffuse", "oren_nayer", "translucent", "phong", "ward", "microfacet",
        "reflection", "transparent", "debug", "holdout", "subsurface", 
        // TODO: Add all OSL standard library functions names
        "mix", "rotate"
    });

    //
    // Register type syntax handlers for each data type.
    //

    registerTypeSyntax
    (
        std::make_shared<ScalarTypeSyntax>(
            RtType::FLOAT, 
            "float",
            "0.0",
            "0.0")
    );

    registerTypeSyntax
    (
        std::make_shared<ScalarTypeSyntax>(
            RtType::INTEGER,
            "int",
            "0",
            "0")
    );

    registerTypeSyntax
    (
        std::make_shared<OslBooleanTypeSyntax>()
    );

    registerTypeSyntax
    (
        // Note: the color type in OSL is a built in type and
        // should not use the custom OslStructTypeSyntax.
        std::make_shared<AggregateTypeSyntax>(
            RtType::COLOR3,
            "color",
            "color(0.0)",
            "color(0.0)",
            EMPTY_STRING,
            EMPTY_STRING,
            VECTOR_MEMBERS)
    );

    registerTypeSyntax
    (
        std::make_shared<OslColor4TypeSyntax>()
    );

    registerTypeSyntax
    (
        std::make_shared<OslStructTypeSyntax>(
            RtType::VECTOR2,
            "vector2",
            "vector2(0.0, 0.0)",
            "{0.0, 0.0}",
            EMPTY_STRING,
            EMPTY_STRING,
            VECTOR2_MEMBERS)
    );

    registerTypeSyntax
    (
        // Note: the vector type in OSL is a built in type and
        // should not use the custom OslStructTypeSyntax.
        std::make_shared<AggregateTypeSyntax>(
            RtType::VECTOR3,
            "vector",
            "vector(0.0)",
            "vector(0.0)",
            EMPTY_STRING,
            EMPTY_STRING,
            VECTOR_MEMBERS)
    );

    registerTypeSyntax
    (
        std::make_shared<OslStructTypeSyntax>(
            RtType::VECTOR4,
            "vector4",
            "vector4(0.0, 0.0, 0.0, 0.0)",
            "{0.0, 0.0, 0.0, 0.0}",
            EMPTY_STRING,
            EMPTY_STRING,
            VECTOR4_MEMBERS)
    );

    registerTypeSyntax
    (
        std::make_shared<OSLMatrix3TypeSyntax>(
            "matrix",
            "matrix(1.0)",
            "matrix(1.0)")
    );

    registerTypeSyntax
    (
        std::make_shared<AggregateTypeSyntax>(
            RtType::MATRIX44,
            "matrix",
            "matrix(1.0)",
            "matrix(1.0)")
    );

    registerTypeSyntax
    (
        std::make_shared<StringTypeSyntax>(
            RtType::STRING,
            "string",
            "\"\"",
            "\"\"")
    );

    registerTypeSyntax
    (
        std::make_shared<StringTypeSyntax>(
            RtType::FILENAME,
            "string",
            "\"\"",
            "\"\"")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::BSDF,
            "BSDF",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::EDF,
            "EDF",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::VDF,
            "VDF",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::SURFACESHADER,
            "surfaceshader",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::VOLUMESHADER,
            "volumeshader",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::DISPLACEMENTSHADER,
            "displacementshader",
            "vector(0.0)",
            "vector(0.0)",
            "vector")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::LIGHTSHADER,
            "lightshader",
            "null_closure",
            "0",
            "closure color")
    );

    registerTypeSyntax
    (
        std::make_shared<SingleValuedTypeSyntax>(
            RtType::THINFILM,
            "thinfilm",
            "thinfilm(0.0, 1.5)",
            EMPTY_STRING,
            EMPTY_STRING,
            "struct thinfilm { float thickness; float ior; };")
    );
}

const string& OslSyntax::getOutputQualifier() const
{
    return OUTPUT_QUALIFIER;
}

} // namespace Codegen
} // namespace MaterialX
