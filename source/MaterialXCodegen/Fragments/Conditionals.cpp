//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragments/Conditionals.h>
#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/Context.h>

namespace MaterialX
{
namespace Codegen
{

IfFragment::IfFragment(const RtToken& name) :
    Fragment(name)
{
}

void IfFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();

    const Output* output = getOutput();
    const Input* value1 = getInput(0);
    const Input* value2 = getInput(1);
    const Input* in1 = getInput(2);
    const Input* in2 = getInput(3);

    result.beginLine();
    compiler.declareVariable(*output, true, result);
    result.endLine();

    result.beginLine();
    result.addString("if (");
    result.addString(compiler.getResult(*value1));
    result.addString(equalityOperator());
    result.addString(compiler.getResult(*value2));
    result.addString(")");
    result.endLine(false);

    result.beginScope();
    result.beginLine();
    result.addString(output->getVariable().str() + " = ");
    result.addString(compiler.getResult(*in1));
    result.endLine();
    result.endScope();
    result.addLine("else", false);
    result.beginScope();
    result.beginLine();
    result.addString(output->getVariable().str() + " = ");
    result.addString(compiler.getResult(*in2));
    result.endLine();
    result.endScope();
}


DEFINE_FRAGMENT_CLASS(IfEqualFragment, IfFragment)

const string& IfEqualFragment::equalityOperator() const
{
    static const string OP(" == ");
    return OP;
}


DEFINE_FRAGMENT_CLASS(IfGreaterFragment, IfFragment)

const string& IfGreaterFragment::equalityOperator() const
{
    static const string OP(" > ");
    return OP;
}


DEFINE_FRAGMENT_CLASS(IfGreaterEqFragment, IfFragment)

const string& IfGreaterEqFragment::equalityOperator() const
{
    static const string OP(" >= ");
    return OP;
}


DEFINE_FRAGMENT_CLASS(SwitchFragment, Fragment)

void SwitchFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();

    const Output* output = getOutput();
    const Input* which = getInput(5);

    // Declare the output variable
    result.beginLine();
    compiler.declareVariable(*output, true, result);
    result.endLine();

    // Process the branches of the switch node
    for (size_t branch = 0; branch < 5; ++branch)
    {
        const Input* input = getInput(branch);
        result.beginLine();
        if (branch > 0)
        {
            result.addString("else ");
        }
        result.addString("if (float(");
        result.addString(compiler.getResult(*which));
        result.addString(") < ");
        result.addValue(float(branch + 1));
        result.addString(")");
        result.endLine(false);
        result.beginScope();
        result.beginLine();
        result.addString(output->getVariable().str() + " = ");
        result.addString(compiler.getResult(*input));
        result.endLine();
        result.endScope();
    }
}

} // namespace Codegen
} // namespace MaterialX
