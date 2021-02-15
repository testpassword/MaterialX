//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/FragmentCompiler.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtApi.h>

#include <MaterialXFormat/File.h>

#include <sstream>
#include <deque>

namespace MaterialX
{
namespace Codegen
{

Fragment::Fragment(const RtToken& name) :
    _name(name),
    _classification(FragmentClassification::TEXTURE),
    _functionName(name)
{
}

Fragment::Input* Fragment::createInput(const RtToken& type, const RtToken& name)
{
    if (getInput(name))
    {
        throw ExceptionRuntimeError("An input named '" + name.str() +
            "' already exists in fragment '" + getName().str() + "'");
    }

    InputPtr input(new Input());
    input->parent = this;
    input->type = type;
    input->name = name;
    input->variable = name;
    input->value = RtValue::createNew(type, _allocator);
    input->connection = nullptr;

    _inputs.add(name, input);

    return input.get();
}

Fragment::Output* Fragment::createOutput(const RtToken& type, const RtToken& name)
{
    if (getOutput(name))
    {
        throw ExceptionRuntimeError("An output named '" + name.str() +
            "' already exists in fragment '" + getName().str() + "'");
    }

    OutputPtr output(new Output());
    output->parent = this;
    output->type = type;
    output->name = name;
    output->variable = name;

    _outputs.add(name, output);

    return output.get();
}


FragmentGraph::FragmentGraph(const RtToken& name) :
    Fragment(name)
{
}

FragmentPtr FragmentGraph::create(const RtToken& name)
{
    return FragmentPtr(new FragmentGraph(name));
}

const RtToken& FragmentGraph::className()
{
    static const RtToken CLASS_NAME("FragmentGraph");
    return CLASS_NAME;
}

void FragmentGraph::addFragment(const FragmentPtr& fragment)
{
    if (getFragment(fragment->getName()))
    {
        throw ExceptionRuntimeError("A fragment named '" + fragment->getName().str() + 
            "' already exists in fragment graph '" + getName().str() + "'");
    }

    _fragments.add(fragment->getName(), fragment);
}

void FragmentGraph::removeFragment(const RtToken& name)
{
    _fragments.remove(name);
}

size_t FragmentGraph::numFragments() const
{
    return _fragments.size();
}

Fragment* FragmentGraph::getFragment(size_t index) const
{
    return _fragments.get(index);
}

Fragment* FragmentGraph::getFragment(const RtToken& name) const
{
    return _fragments.get(name);
}

void FragmentGraph::connect(Fragment::Output* src, Fragment::Input* dst)
{
    dst->connection = src;
    src->connections.insert(dst);
}

void FragmentGraph::connect(const RtToken& srcFragment, const RtToken& srcOutput,
    const RtToken& dstFragment, const RtToken& dstOutput)
{
    Fragment* src = getFragment(srcFragment);
    Fragment* dst = getFragment(dstFragment);
    if (!(src && dst))
    {
        throw ExceptionRuntimeError("Failed to create connection '" + srcFragment.str() + "' -> '" + dstFragment.str() + "'");
    }

    Fragment::Output* output = src->getOutput(srcOutput);
    Fragment::Input* input = src->getInput(dstOutput);
    if (!(output && input))
    {
        throw ExceptionRuntimeError("Failed to create connection '" + srcFragment.str() + "' -> '" + dstFragment.str() + "'");
    }

    connect(output, input);
}

Fragment::Input* FragmentGraph::createInput(const RtToken& type, const RtToken& name)
{
    // Create the external input.
    Fragment::Input* input = Fragment::createInput(type, name);

    // Create the internal socket.
    OutputPtr socket(new Output());
    socket->parent = this;
    socket->type = type;
    socket->name = input->name;
    socket->variable = input->variable;
    _inputSockets.add(socket->name, socket);

    return input;
}

Fragment::Output* FragmentGraph::createOutput(const RtToken& type, const RtToken& name)
{
    // Create the external output.
    Fragment::Output* output = Fragment::createOutput(type, name);

    // Create the internal socket.
    InputPtr socket(new Input());
    socket->parent = this;
    socket->type = type;
    socket->name = output->name;
    socket->variable = output->variable;
    socket->value = RtValue::createNew(type, _allocator);
    socket->connection = nullptr;
    _outputSockets.add(socket->name, socket);

    return output;
}

void FragmentGraph::finalize(const Context& context, bool publishAllInputs)
{
    //
    // Prepare the graph for compilation
    //
    const size_t numFragments = _fragments.size();

    if (publishAllInputs)
    {
        // Create graph inputs for unconnected inputs inside the graph.
        for (size_t i = 0; i < numFragments; ++i)
        {
            Fragment* fragment = _fragments.get(i);
            for (size_t j = 0; j < fragment->numInputs(); ++j)
            {
                Input* input = fragment->getInput(j);
                if (!input->connection)
                {
                    const RtToken name(input->getLongName());
                    Input* external = createInput(input->type, name);
                    Output* socket = getInputSocket(external->name);
                    connect(socket, input);
                }
            }
        }
    }

    const Syntax& syntax = context.getSyntax();

    // Add all reserved words as taken identifiers
    RtTokenMap<size_t> identifiers;
    for (const RtToken& t : context.getReservedWords())
    {
        identifiers[t] = 1;
    }

    // Make sure the function name is a valid identifier.
    _functionName = syntax.createIdentifier(_functionName.str(), identifiers);

    // Create valid identifiers for all inputs and outputs.
    for (size_t i = 0; i < numInputs(); ++i)
    {
        Port* port = getInputSocket(i);
        port->variable = syntax.createIdentifier(port->name.str(), identifiers);
    }
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        Port* port = getOutputSocket(i);
        port->variable = syntax.createIdentifier(port->name.str(), identifiers);
    }
    for (size_t i = 0; i < numFragments; ++i)
    {
        Fragment* fragment = _fragments.get(i);
        for (size_t j = 0; j < fragment->numInputs(); ++j)
        {
            Port* port = fragment->getInput(j);
            port->variable = syntax.createIdentifier(port->getLongName(), identifiers);
        }
        for (size_t j = 0; j < fragment->numOutputs(); ++j)
        {
            Port* port = fragment->getOutput(j);
            port->variable = syntax.createIdentifier(port->getLongName(), identifiers);
        }
    }

    // Sort the fragments in topological order, using Kahn's algorithm
    // to avoid recursion.

    // Calculate in-degrees for all fragments, and enqueue those with degree 0.
    std::unordered_map<Fragment*, int> inDegree(numFragments);
    std::deque<Fragment*> fragmentQueue;
    for (size_t i = 0; i < numFragments; ++i)
    {
        Fragment* fragment = _fragments.get(i);

        int connectionCount = 0;
        for (size_t j = 0; j < fragment->numInputs(); ++j)
        {
            const Input* input = fragment->getInput(j);
            if (input->connection && input->connection->parent != this)
            {
                ++connectionCount;
            }
        }

        inDegree[fragment] = connectionCount;

        if (connectionCount == 0)
        {
            fragmentQueue.push_back(fragment);
        }
    }

    Fragment** sortedFragments = _fragments.order();
    memset(sortedFragments, 0, numFragments * sizeof(Fragment*));
    size_t count = 0;

    while (!fragmentQueue.empty())
    {
        // Pop the queue and add to topological order.
        Fragment* fragment = fragmentQueue.front();
        fragmentQueue.pop_front();

        sortedFragments[count++] = fragment;

        // Find connected nodes and decrease their in-degree,
        // adding node to the queue if in-degrees becomes 0.
        for (size_t i = 0; i<fragment->numOutputs(); ++i)
        {
            const Output* output = fragment->getOutput(i);
            for (const Input* downstream : output->connections)
            {
                if (downstream->parent != this)
                {
                    Fragment* downstreamFragment = downstream->parent;
                    if (--inDegree[downstreamFragment] <= 0)
                    {
                        fragmentQueue.push_back(downstreamFragment);
                    }
                }
            }
        }
    }

    // Check if there was a cycle.
    if (count != _fragments.size())
    {
        throw ExceptionRuntimeError("Encountered a cycle in fragment graph '" + getName().str() + "'");
    }

    // Set classification based on the root fragment.
    Fragment* rootFragment = sortedFragments[count - 1];
    _classification = rootFragment->getClassificationMask();
}

void FragmentGraph::emitFunctionDefinitions(const Context& context, SourceCode& result) const
{
    // Make sure it's not defined already.
    if (result.isDefined(_functionName))
    {
        return;
    }
    result.setDefined(_functionName);

    const FragmentCompiler& compiler = context.getCompiler();

    for (size_t i = 0; i < numFragments(); ++i)
    {
        const Fragment* child = getFragment(i);
        child->emitFunctionDefinitions(context, result);
    }

    result.beginLine();
    result.addString("void " + _functionName.str() + "(");

    string delim = "";

    // Add all inputs
    for (size_t i = 0; i < numInputs(); ++i)
    {
        result.addString(delim);
        const Fragment::Input* input = getInput(i);
        compiler.declareVariable(*input, false, result);
        delim = ", ";
    }

    // Add all outputs
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        result.addString(delim);
        const Fragment::Output* output = getOutput(i);
        result.addString(context.getSyntax().getOutputQualifier());
        compiler.declareVariable(*output, false, result);
        delim = ", ";
    }
    result.addString(")");
    result.endLine(false);

    result.beginScope();

    for (size_t i = 0; i < numFragments(); ++i)
    {
        const Fragment* child = getFragment(i);
        child->emitFunctionCall(context, result);
    }

    // Emit final results
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        const Fragment::Input* outputSocket = getOutputSocket(i);
        result.beginLine();
        result.addString(outputSocket->variable.str() + " = ");
        compiler.emitVariable(*outputSocket, result);
        result.endLine();
    }

    result.endScope();
    result.newLine();
}

void FragmentGraph::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();

    // Declare all outputs.
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        const Fragment::Output* output = getOutput(i);
        result.beginLine();
        compiler.declareVariable(*output, true, result);
        result.endLine();
    }

    result.beginLine();
    result.addString(_functionName.str() + "(");

    string delim = "";

    // Add all inputs
    for (size_t i = 0; i < numInputs(); ++i)
    {
        const Fragment::Input* input = getInput(i);
        result.addString(delim);
        compiler.emitVariable(*input, result);
        delim = ", ";
    }

    // Add all outputs
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        const Fragment::Output* output = getOutput(i);
        result.addString(delim + output->variable.str());
        delim = ", ";
    }
    result.addString(")");
    result.endLine();
}


SourceFragment::SourceFragment(const RtToken& name) :
    Fragment(name),
    _sourceCode(nullptr)
{
}

FragmentPtr SourceFragment::create(const RtToken& name)
{
    return FragmentPtr(new SourceFragment(name));
}

const RtToken& SourceFragment::className()
{
    static const RtToken CLASS_NAME("SourceFragment");
    return CLASS_NAME;
}

void SourceFragment::emitFunctionDefinitions(const Context& context, SourceCode& result) const
{
    if (!isInline() && !result.isDefined(_functionName))
    {
        // Emit the source code definition.
        context.getCompiler().emitBlock(*_sourceCode, result);
        result.newLine();

        // Mark this function as defined in the source code.
        result.setDefined(_functionName);
    }
}

void SourceFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();

    if (isInline())
    {
        // An inline expression.
        const string& source = *_sourceCode;
        const Syntax& syntax = context.getSyntax();

        static const string prefix("{{");
        static const string postfix("}}");

        size_t pos = 0;
        size_t i = source.find_first_of(prefix);
        StringSet localVariableNames;
        SourceCode inlineResult;
        while (i != string::npos)
        {
            inlineResult.addString(source.substr(pos, i - pos));
            size_t j = source.find_first_of(postfix, i + 2);
            if (j == string::npos)
            {
                throw ExceptionRuntimeError("Malformed inline expression in source code for fragment " + getName().str());
            }

            const RtToken variable(source.substr(i + 2, j - i - 2));
            const Fragment::Input* input = getInput(variable);
            if (!input)
            {
                throw ExceptionRuntimeError("Could not find an input named '" + variable.str() + "' on fragment '" + getName().str() + "'");
            }

            if (input->connection)
            {
                compiler.emitVariable(*input, inlineResult);
            }
            else
            {
                // Declare a local variable with this value.
                // TODO: We should make a valid unique identifier name.
                const string variableName = input->getLongName() + "_tmp";
                if (!localVariableNames.count(variableName))
                {
                    result.beginLine();
                    const string& qualifier = syntax.getConstantQualifier();
                    result.addString(qualifier.empty() ? EMPTY_STRING : qualifier + " ");
                    result.addString(syntax.getTypeName(input->type) + " " + input->variable.str() + " = " + syntax.getValue(input->type, input->value));
                    result.endLine();
                    localVariableNames.insert(variableName);
                }
                inlineResult.addString(variableName);
            }

            pos = j + 2;
            i = source.find_first_of(prefix, pos);
        }
        inlineResult.addString(source.substr(pos));

        result.beginLine();
        compiler.declareVariable(*getOutput(), false, result);
        result.addString(" = " + inlineResult.asString());
        result.endLine();

    }
    else
    {
        // Declare all outputs.
        for (size_t i = 0; i < numOutputs(); ++i)
        {
            const Fragment::Output* output = getOutput(i);
            result.beginLine();
            compiler.declareVariable(*output, true, result);
            result.endLine();
        }

        result.beginLine();
        result.addString(_functionName.str() + "(");

        string delim = "";

        // Add all inputs
        for (size_t i = 0; i < numInputs(); ++i)
        {
            const Fragment::Input* input = getInput(i);
            result.addString(delim);
            compiler.emitVariable(*input, result);
            delim = ", ";
        }

        // Add all outputs
        for (size_t i = 0; i < numOutputs(); ++i)
        {
            const Fragment::Output* output = getOutput(i);
            result.addString(delim + output->variable.str());
            delim = ", ";
        }
        result.addString(")");
        result.endLine();
    }
}


} // namespace Codegen
} // namespace MaterialX
