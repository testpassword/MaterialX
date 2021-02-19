//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragments/Convert.h>
#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtTypeDef.h>

namespace MaterialX
{
namespace Codegen
{

namespace
{
    RtTokenMap<RtTokenMap<string>> CONVERT_TABLE =
    {
        {
            RtType::COLOR3,
            {
                { RtType::VECTOR3, string("rgb") },
                { RtType::COLOR4, string("rgb1") }
            }
        },
        {
            RtType::COLOR4,
            {
                { RtType::VECTOR4, string("rgba") },
                { RtType::COLOR3, string("rgb") }
            }
        },
        {
            RtType::VECTOR2,
            {
                { RtType::VECTOR3, string("xy0") }
            }
        },
        {
            RtType::VECTOR3,
            {
                { RtType::COLOR3, string("xyz") },
                { RtType::VECTOR4, string("xyz1") },
                { RtType::VECTOR2, string("xy") }
            }
        },
        {
            RtType::VECTOR4,
            {
                { RtType::COLOR4, string("xyzw") },
                { RtType::VECTOR3, string("xyz") }
            }
        },
        {
            RtType::FLOAT,
            {
                { RtType::COLOR3, string("rrr") },
                { RtType::COLOR4, string("rrrr") },
                { RtType::VECTOR2, string("rr") },
                { RtType::VECTOR3, string("rrr") },
                { RtType::VECTOR4, string("rrrr") }
            }
        }
    };
}

ConvertFragment::ConvertFragment(const RtToken& name) : Fragment(name) {}

FragmentPtr ConvertFragment::create(const RtToken& name)
{
    return FragmentPtr(new ConvertFragment(name));
}

const RtToken& ConvertFragment::className()
{
    static const RtToken CLASS_NAME("ConvertFragment");
    return CLASS_NAME;
}

FragmentPtr ConvertFragment::clone() const
{
    FragmentPtr other = ConvertFragment::create(_name);
    other->copy(*this);
    return other;
}

void ConvertFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();
    const Syntax& syntax = context.getSyntax();

    const Output* out = getOutput();
    const Input* in = getInput(0);

    const RtTypeDef* inType = RtTypeDef::findType(in->getType());
    const RtTypeDef* outType = RtTypeDef::findType(out->getType());

    string converted;

    // Handle scalar type conversions.
    if (inType->isScalar() && outType->isScalar())
    {
        converted = syntax.getTypeName(in->getType()) + "(" + compiler.getResult(*in) + ")";
    }
    // Handle vector type conversions.
    else
    {
        // Search the conversion table for a swizzle pattern to use.
        const string* swizzle = nullptr;
        auto i = CONVERT_TABLE.find(in->getType());
        if (i != CONVERT_TABLE.end())
        {
            auto j = i->second.find(out->getType());
            if (j != i->second.end())
            {
                swizzle = &j->second;
            }
        }
        if (!swizzle || swizzle->empty())
        {
            throw ExceptionRuntimeError("Conversion from '" + in->getType().str() + "' to '" + out->getType().str() + "' is not supported by convert node");
        }

        // If the input is unconnected we must declare a local variable
        // for it first, in order to swizzle it below.
        string variable;
        RtToken type;
        if (!in->isConnected())
        {
            compiler.declareVariable(*in, true, result);
            variable = in->getVariable().str();
            type = in->getType();
        }
        else
        {
            variable = in->getConnection()->getVariable().str();
            type = in->getConnection()->getType();
        }

        converted = syntax.getSwizzledVariable(variable, type, out->getType(), *swizzle);
    }

    result.beginLine();
    compiler.declareVariable(*out, false, result);
    result.addString(" = " + converted);
    result.endLine();
}


SwizzleFragment::SwizzleFragment(const RtToken& name) : Fragment(name) {}

FragmentPtr SwizzleFragment::create(const RtToken& name)
{
    return FragmentPtr(new SwizzleFragment(name));
}

const RtToken& SwizzleFragment::className()
{
    static const RtToken CLASS_NAME("SwizzleFragment");
    return CLASS_NAME;
}

FragmentPtr SwizzleFragment::clone() const
{
    FragmentPtr other = SwizzleFragment::create(_name);
    other->copy(*this);
    return other;
}

void SwizzleFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();
    const Syntax& syntax = context.getSyntax();

    const Output* out = getOutput();
    const Input* in = getInput(0);
    const Input* channels = getInput(1);

    const string& swizzle = channels->getValue().asString();

    // If the input is unconnected we must declare a local variable
    // for it first, in order to swizzle it below.
    string variable;
    RtToken type;
    if (!in->isConnected())
    {
        compiler.declareVariable(*in, true, result);
        variable = in->getVariable().str();
        type = in->getType();
    }
    else
    {
        variable = in->getConnection()->getVariable().str();
        type = in->getConnection()->getType();
    }

    result.beginLine();
    compiler.declareVariable(*out, false, result);
    result.addString(" = " + syntax.getSwizzledVariable(variable, in->getType(), out->getType(), swizzle));
    result.endLine();
}

} // namespace Codegen
} // namespace MaterialX
