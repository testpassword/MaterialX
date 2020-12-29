//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/Codegen/PvtGraphImpl.h>

#include <deque>

namespace MaterialX
{

PvtGraphImpl::PvtGraphImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent)
    : PvtPrim(typeInfo, name, parent)
{
}

PvtGraphImpl::PvtGraphImpl(const RtToken& name)
    : PvtPrim(&RtGraphImpl::_typeInfo, name, nullptr)
{
}

PvtGraphImpl::~PvtGraphImpl()
{
}

void PvtGraphImpl::initialize(const RtNode& node)
{
    clear();
    traverse(node.getOutput());
    sort();
}

void PvtGraphImpl::initialize(const RtNodeGraph& nodegraph, const RtToken& output)
{
    clear();
    if (output != EMPTY_TOKEN)
    {
        // Traverse the given output.
        RtInput socket = nodegraph.getOutputSocket(output);
        if (!socket)
        {
            throw ExceptionRuntimeError("Nodegraph '" + nodegraph.getName().str() + "' has not output named '" + output.str() + "'");
        }
        if (socket.isConnected())
        {
            traverse(socket.getConnection());
        }
    }
    else
    {
        // Traverse all outputs.
        for (RtAttribute attr : nodegraph.getOutputs())
        {
            RtInput socket = nodegraph.getOutputSocket(attr.getName());
            if (socket.isConnected())
            {
                traverse(socket.getConnection());
            }
        }
    }
    sort();
}

PvtDataHandle PvtGraphImpl::createNew(const RtNode& node)
{
    PvtGraphImpl* impl = new PvtGraphImpl(node.getName());
    impl->initialize(node);
    return PvtDataHandle(impl);
}

PvtDataHandle PvtGraphImpl::createNew(const RtNodeGraph& nodegraph, const RtToken& output)
{
    PvtGraphImpl* impl = new PvtGraphImpl(nodegraph.getName());
    impl->initialize(nodegraph, output);
    return PvtDataHandle(impl);
}

void PvtGraphImpl::clear()
{
    _nodes.clear();
    _nodeOrder.clear();
}

void PvtGraphImpl::traverse(const RtOutput& output)
{
    RtGraphIterator graphIterator(output);
    for (RtEdge edge : graphIterator)
    {
        RtPrim prim(edge.upstream.getParent());
        _nodes[prim.getName()] = PvtObject::ptr<PvtPrim>(prim);
    }
}

void PvtGraphImpl::sort()
{
    // Sort the nodes in topological order, using Kahn's algorithm
    // to avoid recursion.
    //
    // Running time: O(numNodes + numEdges).

    // Calculate in-degrees for all nodes, and enqueue those with degree 0.
    std::unordered_map<PvtPrim*, int> inDegree(_nodes.size());
    std::deque<PvtPrim*> nodeQueue;
    for (const auto& it : _nodes)
    {
        PvtPrim* node = it.second;

        int connectionCount = 0;
        for (RtAttribute attr : node->getInputs())
        {
            RtInput input = attr.asA<RtInput>();
            if (input.isConnected() && !input.getConnection().isSocket())
            {
                ++connectionCount;
            }
        }

        inDegree[node] = connectionCount;

        if (connectionCount == 0)
        {
            nodeQueue.push_back(node);
        }
    }

    _nodeOrder.resize(_nodes.size(), nullptr);
    size_t count = 0;

    while (!nodeQueue.empty())
    {
        // Pop the queue and add to topological order.
        PvtPrim* node = nodeQueue.front();
        nodeQueue.pop_front();
        _nodeOrder[count++] = node;

        // Find connected nodes and decrease their in-degree,
        // adding node to the queue if in-degrees becomes 0.
        for (const auto& attr : node->getOutputs())
        {
            RtOutput output = attr.asA<RtOutput>();
            for (const auto& downstream : output.getConnections())
            {
                RtInput downstreamInput = downstream.asA<RtInput>();
                if (!downstreamInput.isSocket())
                {
                    PvtPrim* downstreamNode = PvtObject::ptr<PvtPrim>(downstreamInput.getParent());
                    if (--inDegree[downstreamNode] <= 0)
                    {
                        nodeQueue.push_back(downstreamNode);
                    }
                }
            }
        }
    }

    // Check if there was a cycle.
    if (count != _nodes.size())
    {
        throw ExceptionRuntimeError("Encountered a cycle in graph for graphimpl: " + getName().str());
    }
}

}
