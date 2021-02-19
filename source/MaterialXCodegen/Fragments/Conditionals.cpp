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


IfEqualFragment::IfEqualFragment(const RtToken& name) : IfFragment(name) {}

FragmentPtr IfEqualFragment::create(const RtToken& name)
{
    return FragmentPtr(new IfEqualFragment(name));
}

const RtToken& IfEqualFragment::className()
{
    static const RtToken CLASS_NAME("IfEqualFragment");
    return CLASS_NAME;
}

FragmentPtr IfEqualFragment::clone() const
{
    FragmentPtr other = IfEqualFragment::create(_name);
    other->copy(*this);
    return other;
}

const string& IfEqualFragment::equalityOperator() const
{
    static const string OP(" == ");
    return OP;
}


IfGreaterFragment::IfGreaterFragment(const RtToken& name) : IfFragment(name) {}

FragmentPtr IfGreaterFragment::create(const RtToken& name)
{
    return FragmentPtr(new IfGreaterFragment(name));
}

const RtToken& IfGreaterFragment::className()
{
    static const RtToken CLASS_NAME("IfGreaterFragment");
    return CLASS_NAME;
}

FragmentPtr IfGreaterFragment::clone() const
{
    FragmentPtr other = IfGreaterFragment::create(_name);
    other->copy(*this);
    return other;
}

const string& IfGreaterFragment::equalityOperator() const
{
    static const string OP(" > ");
    return OP;
}


IfGreaterEqFragment::IfGreaterEqFragment(const RtToken& name) : IfFragment(name) {}

FragmentPtr IfGreaterEqFragment::create(const RtToken& name)
{
    return FragmentPtr(new IfGreaterFragment(name));
}

const RtToken& IfGreaterEqFragment::className()
{
    static const RtToken CLASS_NAME("IfGreaterEqFragment");
    return CLASS_NAME;
}

FragmentPtr IfGreaterEqFragment::clone() const
{
    FragmentPtr other = IfGreaterEqFragment::create(_name);
    other->copy(*this);
    return other;
}

const string& IfGreaterEqFragment::equalityOperator() const
{
    static const string OP(" >= ");
    return OP;
}

} // namespace Codegen
} // namespace MaterialX
