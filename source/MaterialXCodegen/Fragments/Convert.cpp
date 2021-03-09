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

DEFINE_FRAGMENT_CLASS(ConvertFragment, Fragment)

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


DEFINE_FRAGMENT_CLASS(SwizzleFragment, Fragment)

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


DEFINE_FRAGMENT_CLASS(CombineFragment, Fragment)

void CombineFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();
    const Syntax& syntax = context.getSyntax();

    const Input* in1 = getInput(0);
    const Output* out = getOutput();

    // Check the node signature to determine which
    // type conversion to perform, and get the value
    // components to use for constructing the new value.
    //
    StringVec valueComponents;
    if (in1->getType() == RtType::FLOAT)
    {
        // Get the components of the input values.
        const size_t cnt = numInputs();
        valueComponents.resize(cnt);
        for (size_t i = 0; i < cnt; ++i)
        {
            const Input* input = getInput(i);
            valueComponents[i] = compiler.getResult(*input);
        }
    }
    else if (in1->getType() == RtType::COLOR3 || in1->getType() == RtType::VECTOR3)
    {
        const Input* in2 = getInput(1);

        // If in1 is unconnected we must declare a local variable
        // for it first, in order to access components from it below.
        const RtToken& in1Variable = in1->isConnected() ? in1->getConnection()->getVariable() : in1->getVariable();
        if (!in1->isConnected())
        {
            string variableValue = syntax.getValue(in1->getType(), in1->getValue());
            result.addLine(syntax.getTypeName(in1->getType()) + " " + in1Variable.str() + " = " + variableValue);
        }

        // Get components from in1
        const StringVec& in1Members = syntax.getTypeSyntax(in1->getType()).getMembers();
        size_t memberSize = in1Members.size();

        // Get the components of the input values.
        if (memberSize)
        {
            valueComponents.resize(memberSize + 1);
            for (size_t i = 0; i < memberSize; i++)
            {
                valueComponents[i] = in1Variable.str() + in1Members[i];
            }
        }
        else
        {
            memberSize = 1;
            valueComponents.resize(2);
            valueComponents[0] = in1Variable.str();
        }
        // Get component from in2
        valueComponents[memberSize] = compiler.getResult(*in2);
    }
    else if (in1->getType() == RtType::VECTOR2)
    {
        const Input* in2 = getInput(1);

        // If in1 is unconnected we must declare a local variable
        // for it first, in order to access components from it below.
        const RtToken& in1Variable = in1->isConnected() ? in1->getConnection()->getVariable() : in1->getVariable();
        if (!in1->isConnected())
        {
            const string variableValue = syntax.getValue(in1->getType(), in1->getValue());
            result.addLine(syntax.getTypeName(in1->getType()) + " " + in1Variable.str() + " = " + variableValue);
        }

        // If in2 is unconnected we must declare a local variable
        // for it first, in order to access components from it below.
        const RtToken&  in2Variable = in2->isConnected() ? in2->getConnection()->getVariable() : in2->getVariable();
        if (!in2->isConnected())
        {
            const string variableValue = syntax.getValue(in2->getType(), in2->getValue());
            result.addLine(syntax.getTypeName(in2->getType()) + " " + in2Variable.str() + " = " + variableValue);
        }

        // Get the components of the input values.
        valueComponents.resize(4);

        // Get components from in1.
        const StringVec& in1Members = syntax.getTypeSyntax(in1->getType()).getMembers();
        valueComponents[0] = in1Variable.str() + in1Members[0];
        valueComponents[1] = in1Variable.str() + in1Members[1];

        // Get components from in2.
        const StringVec& in2Members = syntax.getTypeSyntax(in2->getType()).getMembers();
        valueComponents[2] = in2Variable.str() + in2Members[0];
        valueComponents[3] = in2Variable.str() + in2Members[1];
    }

    // Let the TypeSyntax construct the value from the components.
    const TypeSyntax& outTypeSyntax = syntax.getTypeSyntax(out->getType());
    result.beginLine();
    compiler.declareVariable(*out, false, result);
    result.addString(" = " + outTypeSyntax.getValue(valueComponents));
    result.endLine();
}

} // namespace Codegen
} // namespace MaterialX
