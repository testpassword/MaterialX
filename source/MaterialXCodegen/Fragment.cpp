//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/FragmentGenerator.h>
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

void FragmentGraph::finalize()
{
    const size_t numFragments = _fragments.size();

    // Create graph inputs for unconnected inputs inside the graph.
    for (size_t i = 0; i < numFragments; ++i)
    {
        Fragment* fragment = _fragments.get(i);
        for (size_t j = 0; j < fragment->numInputs(); ++j)
        {
            Input* input = fragment->getInput(j);
            if (!input->connection)
            {
                // Use a consistent naming convention: <fragmentname>_<inputname>
                // so application side can figure out what uniforms to set
                // when node inputs change on application side.
                const string validName = fragment->getName().str() + "_" + input->name.str();
                const RtToken name(validName);
                Input* external = createInput(input->type, name);
                Output* socket = getInputSocket(external->name);
                connect(socket, input);
            }
        }
    }

    // Make unique valid variable names.
    for (size_t i = 0; i < numFragments; ++i)
    {
        Fragment* fragment = _fragments.get(i);
        for (size_t j = 0; j < fragment->numInputs(); ++j)
        {
            Port* port = fragment->getInput(j);
            string validName = fragment->getName().str() + "_" + port->name.str();
            port->variable = RtToken(validName);
        }
        for (size_t j = 0; j < fragment->numOutputs(); ++j)
        {
            Port* port = fragment->getOutput(j);
            string validName = fragment->getName().str() + "_" + port->name.str();
            port->variable = RtToken(validName);
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
            if (input->connection && input->connection->parent->getType() != FRAGMENT_TYPE_GRAPH)
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
                if (downstream->parent->getType() != FRAGMENT_TYPE_GRAPH)
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
}

} // namespace Codegen
} // namespace MaterialX
