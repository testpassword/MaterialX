//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/Codegen/PvtGraphImpl.h>

#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>

#include <deque>

namespace MaterialX
{

PvtGraphImpl::PvtGraphImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent)
    : PvtCodegenImpl(typeInfo, name, parent)
{
}

PvtGraphImpl::PvtGraphImpl(const RtToken& name)
    : PvtCodegenImpl(&RtGraphImpl::_typeInfo, name, nullptr)
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

void PvtGraphImpl::emitFunctionDefinition(const RtNode& /*node*/, RtCodegenContext& context, RtFragment& frag) const
{
    const RtCodeGenerator* gen = context.getCodeGenerator();

    // Emit functions for all child nodes.
    for (const PvtPrim* p : _nodeOrder)
    {
        RtNode node(p->hnd());
        RtNodeDef nodedef(node.getNodeDef());
        RtCodegenImpl nodeimpl = gen->getCodegenImpl(nodedef);
        nodeimpl.emitFunctionDefinition(node, context, frag);
    }

    string functionName = getName().str();
    frag.getSyntax().makeValidName(functionName);

    // Begin function signature.
    frag.beginLine();
    frag.addString("void " + functionName + +"(");

    string delim = "";
/*
    // Add all inputs
    for (ShaderGraphInputSocket* inputSocket : _rootGraph->getInputSockets())
    {
        shadergen.emitString(delim + syntax.getTypeName(inputSocket->getType()) + " " + inputSocket->getVariable(), stage);
        delim = ", ";
    }

    // Add all outputs
    for (ShaderGraphOutputSocket* outputSocket : _rootGraph->getOutputSockets())
    {
        shadergen.emitString(delim + syntax.getOutputTypeName(outputSocket->getType()) + " " + outputSocket->getVariable(), stage);
        delim = ", ";
    }
*/
    // End function signature.
    frag.addString(")");
    frag.endLine(false);

    // Begin function body.
    frag.beginScope();

    for (const PvtPrim* p : _nodeOrder)
    {
        RtNode node(p->hnd());
        RtNodeDef nodedef(node.getNodeDef());
        RtCodegenImpl nodeimpl = gen->getCodegenImpl(nodedef);
        nodeimpl.emitFunctionCall(node, context, frag);
    }

    /*
    frag.emitFunctionCalls(*_rootGraph, context, stage);

    // Emit final results
    for (ShaderGraphOutputSocket* outputSocket : _rootGraph->getOutputSockets())
    {
        const string result = shadergen.getUpstreamResult(outputSocket, context);
        shadergen.emitLine(outputSocket->getVariable() + " = " + result, stage);
    }
    */
    // End function body.
    frag.endScope();
    frag.newLine();
}

void PvtGraphImpl::emitFunctionCall(const RtNode& /*node*/, RtCodegenContext& /*context*/, RtFragment& frag) const
{
    frag.addLine("Call on me");
}

}
