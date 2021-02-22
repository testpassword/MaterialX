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
    NamedObject(name),
    _parent(nullptr),
    _classification(FragmentClassification::TEXTURE),
    _functionName(name)
{
}

void Fragment::copy(const Fragment& other)
{
    _name = other._name;
    _classification = other._classification;
    _functionName = other._functionName;

    _inputs.clear();
    _outputs.clear();

    for (size_t i = 0; i < other.numInputs(); ++i)
    {
        const Input* port = other.getInput(i);
        Input* otherPort = createInput(port->getName(), port->getType());
        otherPort->setVariable(port->getVariable());
        RtValue::copy(port->getType(), port->getValue(), otherPort->getValue());
    }
    for (size_t i = 0; i < other.numOutputs(); ++i)
    {
        const Output* port = other.getOutput(i);
        Output* otherPort = createOutput(port->getName(), port->getType());
        otherPort->setVariable(port->getVariable());
    }
}

Input* Fragment::createInput(const RtToken& name, const RtToken& type)
{
    if (getInput(name))
    {
        throw ExceptionRuntimeError("An input named '" + name.str() +
            "' already exists in fragment '" + getName().str() + "'");
    }

    InputPtr input(new Input(name, type));
    input->setParent(this);
    input->getValue() = RtValue::createNew(type, _allocator);

    _inputs.add(input);

    return input.get();
}

Output* Fragment::createOutput(const RtToken& name, const RtToken& type)
{
    if (getOutput(name))
    {
        throw ExceptionRuntimeError("An output named '" + name.str() +
            "' already exists in fragment '" + getName().str() + "'");
    }

    OutputPtr output(new Output(name, type));
    output->setParent(this);
    output->setVariable(name);

    _outputs.add(output);

    return output.get();
}

void Fragment::emitFunctionDefinitions(const Context&, SourceCode&) const
{
}

void Fragment::emitFunctionCall(const Context&, SourceCode&) const
{
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

FragmentPtr FragmentGraph::clone() const
{
    FragmentPtr other = FragmentGraph::create(_name);
    other->copy(*this);
    return other;
}

void FragmentGraph::copy(const Fragment& /*other*/)
{
    throw ExceptionRuntimeError("FragmentGraph::copy: Not implemented yet!");
/*
    Fragment::copy(other);

    FragmentGraph* otherGraph = other.asA<FragmentGraph>();
    otherGraph->_fragments.clear();
    for (size_t i = 0; i < _fragments.size(); ++i)
    {
        Fragment* fragment = _fragments.get(i);
        otherGraph->addFragment(fragment->clone());
    }

    for (size_t i = 0; i < _fragments.size(); ++i)
    {
        Fragment* fragment = _fragments.get(i);
        
    }
*/
}

void FragmentGraph::addFragment(FragmentPtr fragment)
{
    if (getFragment(fragment->getName()))
    {
        throw ExceptionRuntimeError("A fragment named '" + fragment->getName().str() + 
            "' already exists in fragment graph '" + getName().str() + "'");
    }
    fragment->setParent(this);
    _fragments.add(fragment);
}

FragmentPtr FragmentGraph::removeFragment(const RtToken& name)
{
    FragmentPtr frag = _fragments.remove(name);
    if (frag)
    {
        frag->setParent(nullptr);
    }
    return frag;
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

void FragmentGraph::connect(Output* src, Input* dst)
{
    dst->_connection = src;
    src->_connections.insert(dst);
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

    Output* output = src->getOutput(srcOutput);
    Input* input = src->getInput(dstOutput);
    if (!(output && input))
    {
        throw ExceptionRuntimeError("Failed to create connection '" + srcFragment.str() + "' -> '" + dstFragment.str() + "'");
    }

    connect(output, input);
}

Input* FragmentGraph::createInput(const RtToken& name, const RtToken& type)
{
    // Create the external input.
    Input* input = Fragment::createInput(name, type);

    // Create the internal socket.
    OutputPtr socket(new Output(name, type));
    socket->setParent(this);
    _inputSockets.add(socket);

    return input;
}

Output* FragmentGraph::createOutput(const RtToken& name, const RtToken& type)
{
    // Create the external output.
    Output* output = Fragment::createOutput(name, type);

    // Create the internal socket.
    InputPtr socket(new Input(name, type));
    socket->setParent(this);
    socket->getValue() = RtValue::createNew(type, _allocator);
    _outputSockets.add(socket);

    return output;
}

void FragmentGraph::finalize(const Context& context, bool publishAllInputs)
{
    //
    // Finalize the graph construction.
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
                if (!input->isConnected())
                {
                    const RtToken name(input->getLongName());
                    Input* external = createInput(name, input->getType());
                    Output* socket = getInputSocket(external->getName());
                    connect(socket, input);
                }
            }
        }
    }

    const Syntax& syntax = context.getSyntax();

    // Add all reserved words as taken identifiers
    RtTokenMap<size_t> identifiers;
    for (const RtToken& word : context.getCompiler().getReservedWords())
    {
        identifiers[word] = 1;
    }

    // Make sure the function name is a valid identifier.
    _functionName = syntax.createIdentifier(_functionName.str(), identifiers);

    // Create valid identifiers for all inputs and outputs.
    for (size_t i = 0; i < numInputs(); ++i)
    {
        Input* input = getInput(i);
        Output* socket = getInputSocket(i);
        input->setVariable(syntax.createIdentifier(input->getName().str(), identifiers));
        socket->setVariable(input->getVariable());
    }
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        Output* output = getOutput(i);
        Input* socket = getOutputSocket(i);
        output->setVariable(syntax.createIdentifier(output->getName().str(), identifiers));
        socket->setVariable(output->getVariable());
    }
    for (size_t i = 0; i < numFragments; ++i)
    {
        Fragment* fragment = _fragments.get(i);
        for (size_t j = 0; j < fragment->numInputs(); ++j)
        {
            Input* input = fragment->getInput(j);
            input->setVariable(syntax.createIdentifier(input->getLongName(), identifiers));
        }
        for (size_t j = 0; j < fragment->numOutputs(); ++j)
        {
            Output* output = fragment->getOutput(j);
            output->setVariable(syntax.createIdentifier(output->getLongName(), identifiers));
        }
    }

    //
    // Sort the fragments in topological order, using Kahn's algorithm
    // to avoid recursion.
    //

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
            if (input->isConnected() && input->getConnection()->getParent() != this)
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

    ObjectList<Fragment> orderedFragments;
    while (!fragmentQueue.empty())
    {
        // Pop the queue and add to topological order.
        Fragment* fragment = fragmentQueue.front();
        fragmentQueue.pop_front();

        orderedFragments.add(fragment->shared_from_this());

        // Find connected nodes and decrease their in-degree,
        // adding node to the queue if in-degrees becomes 0.
        for (size_t i = 0; i<fragment->numOutputs(); ++i)
        {
            const Output* output = fragment->getOutput(i);
            for (const Input* downstream : output->getConnections())
            {
                Fragment* downstreamFragment = downstream->getParent();
                if (downstreamFragment != this)
                {
                    if (--inDegree[downstreamFragment] <= 0)
                    {
                        fragmentQueue.push_back(downstreamFragment);
                    }
                }
            }
        }
    }

    // Check if there was a cycle.
    if (orderedFragments.size() != _fragments.size())
    {
        throw ExceptionRuntimeError("Encountered a cycle in fragment graph '" + getName().str() + "'");
    }

    _fragments = orderedFragments;

    // Set classification based on the root fragment.
    Fragment* rootFragment = orderedFragments.get(orderedFragments.size() - 1);
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

    // Add all input sockets
    for (size_t i = 0; i < numInputs(); ++i)
    {
        result.addString(delim);
        const Output* socket = getInputSocket(i);
        compiler.declareVariable(*socket, false, result);
        delim = ", ";
    }

    // Add all output sockets
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        result.addString(delim);
        const Input* socket = getOutputSocket(i);
        result.addString(context.getSyntax().getOutputQualifier());
        compiler.declareVariable(*socket, false, result);
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
        const Input* outputSocket = getOutputSocket(i);
        result.beginLine();
        result.addString(outputSocket->getVariable().str() + " = ");
        result.addString(compiler.getResult(*outputSocket));
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
        const Output* output = getOutput(i);
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
        const Input* input = getInput(i);
        result.addString(delim);
        result.addString(compiler.getResult(*input));
        delim = ", ";
    }

    // Add all outputs
    for (size_t i = 0; i < numOutputs(); ++i)
    {
        const Output* output = getOutput(i);
        result.addString(delim + output->getVariable().str());
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

FragmentPtr SourceFragment::clone() const
{
    FragmentPtr other = SourceFragment::create(_name);
    other->copy(*this);
    return other;
}

void SourceFragment::copy(const Fragment& other)
{
    Fragment::copy(other);
    const SourceFragment* otherFragment = other.asA<const SourceFragment>();
    _sourceCode = otherFragment->_sourceCode;
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
            const Input* input = getInput(variable);
            if (!input)
            {
                throw ExceptionRuntimeError("Could not find an input named '" + variable.str() + "' on fragment '" + getName().str() + "'");
            }

            if (input->isConnected())
            {
                inlineResult.addString(compiler.getResult(*input));
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
                    result.addString(syntax.getTypeName(input->getType()) + " " + variableName + " = " + syntax.getValue(input->getType(), input->getValue()));
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
            const Output* output = getOutput(i);
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
            const Input* input = getInput(i);
            result.addString(delim);
            result.addString(compiler.getResult(*input));
            delim = ", ";
        }

        // Add all outputs
        for (size_t i = 0; i < numOutputs(); ++i)
        {
            const Output* output = getOutput(i);
            result.addString(delim + output->getVariable().str());
            delim = ", ";
        }
        result.addString(")");
        result.endLine();
    }
}


} // namespace Codegen
} // namespace MaterialX
