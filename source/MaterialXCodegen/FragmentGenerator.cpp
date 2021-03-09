//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/FragmentGenerator.h>
#include <MaterialXCodegen/Fragment.h>
#include <MaterialXCodegen/Context.h>

#include <MaterialXRuntime/RtApi.h>
#include <MaterialXRuntime/RtNodeGraph.h>
#include <MaterialXRuntime/RtNodeImpl.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXFormat/File.h>
#include <MaterialXFormat/Util.h>

#include <MaterialXGenShader/Util.h>

#include <sstream>

namespace MaterialX
{
namespace Codegen
{

FragmentGenerator::FragmentGenerator(const Context& context) :
    _context(context)
{
    // Register the default fragment types.
    registerFragmentClass(FragmentGraph::className(), FragmentGraph::create);
    registerFragmentClass(SourceFragment::className(), SourceFragment::create);
}

FragmentPtr FragmentGenerator::createFragment(const RtToken& className, const RtToken& name) const
{
    auto it = _creatorFunctions.find(className);
    return it != _creatorFunctions.end() ? it->second(name) : nullptr;
}

FragmentPtr FragmentGenerator::createFragment(const RtNodeDef& nodedef, const RtToken& name) const
{
    const RtToken& target = _context.getTarget();

    RtPrim nodeImplPrim = nodedef.getNodeImpl(target);
    if (!(nodeImplPrim && nodeImplPrim.hasApi<RtNodeImpl>()))
    {
        throw ExceptionRuntimeError("No valid implementation found for nodedef '" +
            nodedef.getName().str() + "' and target '" + target.str() + "'");
    }

    RtNodeImpl nodeimpl(nodeImplPrim);

    FragmentPtr frag;

    // Check if this is a nodegraph implementation.
    RtNodeGraph nodegraph = nodeimpl.getNodeGraph();
    if (nodegraph)
    {
        // [TODO] Don't recreate every time!
        // Compile and cache the source code on the RtNodeImpl,
        // turning this into a source code fragment instead?
         frag = createFragmentGraph(nodegraph, name);
    }
    else
    {
        // See if a fragment class has been registered for the nodeimpl.
        frag = createFragment(nodeimpl.getName(), name);
        if (!frag)
        {
            // No fragment of this class has been registered so
            // fallback to a source fragment as default class.
            frag = SourceFragment::create(name);
        }

        // Create fragment ports according to the nodedef.
        for (RtAttribute attr : nodedef.getInputs())
        {
            Input* input = frag->createInput(attr.getName(), attr.getType());
            RtValue::copy(input->getType(), attr.getValue(), input->getValue());
        }
        for (RtAttribute attr : nodedef.getOutputs())
        {
            frag->createOutput(attr.getName(), attr.getType());
        }

        const RtToken& function = nodeimpl.getFunction();
        frag->setFunctionName(function);
        frag->setClassification(getClassificationMask(nodedef));

        // Load source code for source code fragments.
        if (frag->isA<SourceFragment>())
        {
            SourceFragment* sourceFragment = frag->asA<SourceFragment>();

            // Get the source code from metadata on the nodeimpl.
            RtTypedValue* sourceCodeData = nodeimpl.addMetadata(Tokens::SOURCECODE, RtType::STRING);
            string* contentPtr = &sourceCodeData->getValue().asString();
            if (contentPtr->empty())
            {
                // No source given so try loading it from file.
                const FilePath path = RtApi::get().getSearchPath().find(nodeimpl.getFile());
                *contentPtr = readFile(path);
                if (contentPtr->empty())
                {
                    throw ExceptionRuntimeError("Failed to get source code from file '" + path.asString() +
                        "' used by implementation '" + nodeimpl.getName().str() + "'");
                }

                // Remove newline if it's an inline expression.
                if (sourceFragment->isInline())
                {
                    contentPtr->erase(std::remove(contentPtr->begin(), contentPtr->end(), '\n'), contentPtr->end());
                }
            }

            // Assign this source code to the fragment.
            sourceFragment->setSourceCode(contentPtr);
        }

        // Finalize the fragment construction.
        frag->finalize(_context);
    }

    return frag;
}

FragmentPtr FragmentGenerator::createFragment(const RtGeomPropDef& geomprop, const RtToken& name) const
{
    // Find the nodedef for the geometric node referenced by the geomprop. Use the type of the
    // input here and ignore the type of the geomprop. They are required to have the same type.
    const RtToken geomNodeDefName("ND_" + geomprop.getGeomProp().str() + "_" + geomprop.getType().str());

    RtPrim geomNodeDefPrim = RtApi::get().getNodeDef(geomNodeDefName);
    if (!geomNodeDefPrim)
    {
        throw ExceptionShaderGenError("Could not find a nodedef named '" + geomNodeDefName.str() +
            "' for defaultgeomprop '" + geomprop.getName().str() + "'");
    }

    // Create the fragment from the nodedef.
    RtNodeDef geomNodeDef(geomNodeDefPrim);
    return createFragment(geomNodeDef, name);
}

FragmentPtr FragmentGenerator::createFragment(const RtNode& node, FragmentGraph& parent) const
{
    FragmentPtr frag;

    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        // Create the fragment from the nodegraph.
        frag = createFragmentGraph(node, node.getName());
    }
    else
    {
        RtPrim nodedefPrim = node.getNodeDef();
        if (!(nodedefPrim && nodedefPrim.hasApi<RtNodeDef>()))
        {
            throw ExceptionRuntimeError("No valid nodedef found for node '" + node.getName().str() + "'");
        }

        // Create the fragment from the nodedef.
        RtNodeDef nodedef(nodedefPrim);
        frag = createFragment(nodedef, node.getName());

        // Set values from the node instance.
        for (RtAttribute attr : node.getInputs())
        {
            Input* input = frag->getInput(attr.getName());
            RtValue::copy(input->getType(), attr.getValue(), input->getValue());
        }
    }

    // Let the parent take ownership of the fragment.
    parent.addFragment(frag);

    // Create any sub-fragments. Returning the resulting fragment graph
    // if this is needed, or return the original fragment otherwise.
    frag = createSubFragments(node, *frag);

    // Check for geomprop usage.
    for (RtAttribute port : node.getInputs())
    {
        if (!port.asA<RtInput>().isConnected())
        {
            const RtTypedValue* defaultGeomProp = port.getMetadata(Tokens::DEFAULTGEOMPROP, RtType::TOKEN);
            if (defaultGeomProp)
            {
                // Find the corresponding geompropdef prim.
                RtPrim prim = RtApi::get().getGeomPropDef(defaultGeomProp->getValue().asToken());
                Input* input = frag->getInput(port.getName());
                if (prim && !input->isConnected())
                {
                    RtGeomPropDef geomPropDef(prim);
                    const RtToken fragmentName("geomprop_" + geomPropDef.getName().str());
                    Fragment* geomPropFragment = parent.getFragment(fragmentName);
                    if (!geomPropFragment)
                    {
                        // Fragment does not exists so create it.
                        FragmentPtr newFrag = createFragment(geomPropDef, fragmentName);
                        parent.addFragment(newFrag);
                        geomPropFragment = newFrag.get();
                    }

                    // Connect the fragment to this input.
                    parent.connect(geomPropFragment->getOutput(), input);
                }
            }
        }
    }

    return frag;
}

FragmentPtr FragmentGenerator::createFragmentGraph(const RtNode& node, const RtToken& name, const RtToken& output, bool publishFreeInputs) const
{
    FragmentPtr frag;
    FragmentGraph* graph;

    if (node.getPrim().hasApi<RtNodeGraph>())
    {
        // Create a fragment graph from the given nodegraph.
        RtNodeGraph nodegraph(node.getPrim());
        frag = FragmentGraph::create(name);
        graph = frag->asA<FragmentGraph>();

        // Create all inputs.
        for (RtAttribute port : nodegraph.getInputs())
        {
            Input* input = graph->createInput(port.getName(), port.getType());
            RtValue::copy(input->getType(), port.getValue(), input->getValue());
        }

        if (output != EMPTY_TOKEN)
        {
            // Create specified output.
            RtOutput port = nodegraph.getOutput(output);
            if (!port)
            {
                throw ExceptionRuntimeError("No output named '" + output.str() + "' on nodegraph '" + nodegraph.getName().str() + "'");
            }
            graph->createOutput(port.getName(), port.getType());
        }
        else
        {
            // Create all outputs.
            for (RtAttribute port : nodegraph.getOutputs())
            {
                graph->createOutput(port.getName(), port.getType());
            }
        }

        // Create child fragments for all nodes in the graph.
        for (RtPrim child : nodegraph.getNodes())
        {
            createFragment(RtNode(child), *graph);
        }

        // Connect all child fragments to each other and to 
        // the fragment graph input interface.
        for (RtPrim child : nodegraph.getNodes())
        {
            RtNode childNode(child);
            Fragment* childFragment = graph->getFragment(childNode.getName());

            for (RtAttribute attr : childNode.getInputs())
            {
                RtInput input = attr.asA<RtInput>();
                RtOutput upstream = input.getConnection();
                if (upstream)
                {
                    Input* fragmentInput = childFragment->getInput(input.getName());
                    if (upstream.isSocket())
                    {
                        Output* graphSocket = graph->getInputSocket(upstream.getName());
                        graph->connect(graphSocket, fragmentInput);
                    }
                    else
                    {
                        Fragment* upstreamChildFragment = graph->getFragment(upstream.getParent().getName());
                        Output* upstreamChildOutput = upstreamChildFragment->getOutput(upstream.getName());
                        graph->connect(upstreamChildOutput, fragmentInput);
                    }
                }
            }
        }

        // Connect all child fragments to the fragment graph output interface.
        for (RtAttribute attr : nodegraph.getOutputs())
        {
            RtInput socket = nodegraph.getOutputSocket(attr.getName());
            RtOutput upstream = socket.getConnection();
            if (upstream)
            {
                Fragment* upstreamChildFragment = graph->getFragment(upstream.getParent().getName());
                Output* upstreamChildOutput = upstreamChildFragment->getOutput(upstream.getName());
                Input* fragmentSocket = graph->getOutputSocket(attr.getName());
                graph->connect(upstreamChildOutput, fragmentSocket);
            }
        }
    }
    else
    {
        // Create a fragment graph by traversing upstream dependencies from the given node.
        
        // Create the container fragment graph.
        frag = FragmentGraph::create(name);
        graph = frag->asA<FragmentGraph>();

        FragmentPtr childFrag = createFragment(node, *graph);

        // Create all inputs.
        for (RtAttribute port : node.getInputs())
        {
            RtInput nodeInput = port.asA<RtInput>();
            if (!nodeInput.isConnected())
            {
                Input* graphInput = graph->createInput(port.getName(), port.getType());
                Input* childInput = childFrag->getInput(port.getName());
                graph->connect(graph->getInputSocket(port.getName()), childInput);
                RtValue::copy(childInput->getType(), childInput->getValue(), graphInput->getValue());
            }
        }

        if (output != EMPTY_TOKEN)
        {
            // Create specified output.
            RtOutput port = node.getOutput(output);
            if (!port)
            {
                throw ExceptionRuntimeError("No output named '" + output.str() + "' on node '" + node.getName().str() + "'");
            }
            graph->createOutput(port.getName(), port.getType());
            graph->connect(childFrag->getOutput(), graph->getOutputSocket());
        }
        else
        {
            // Create all outputs.
            for (RtAttribute port : node.getOutputs())
            {
                graph->createOutput(port.getName(), port.getType());
                graph->connect(childFrag->getOutput(port.getName()), graph->getOutputSocket(port.getName()));
            }
        }
    }

    // If requested create graph inputs for all free/unconnected inputs inside the graph.
    if (publishFreeInputs)
    {
        const size_t numFragments = graph->numFragments();
        for (size_t i = 0; i < numFragments; ++i)
        {
            Fragment* child = graph->getFragment(i);
            const size_t numInputs = child->numInputs();
            for (size_t j = 0; j < numInputs; ++j)
            {
                Input* input = child->getInput(j);
                if (!input->isConnected())
                {
                    const RtToken inputName(input->getLongName());
                    Input* external = graph->createInput(inputName, input->getType());
                    Output* socket = graph->getInputSocket(external->getName());
                    graph->connect(socket, input);
                }
            }
        }
    }

    // Finalize creation.
    frag->finalize(_context);

    return frag;
}

FragmentPtr FragmentGenerator::createSubFragments(const RtNode& node, Fragment& fragment) const
{
    // By default return the same fragment.
    FragmentPtr frag = fragment.shared_from_this();

    const ColorManagementSystem* cms = _context.getColorManagementSystem();
    const RtToken targetSpace = _context.getOptions().targetColorSpaceOverride;

    // Check for color management usage.
    if (cms && targetSpace != EMPTY_TOKEN)
    {
        std::unordered_map<Input*, ColorSpaceTransform> inputsWithColorSpaceTransforms;
        std::unordered_map<Output*, ColorSpaceTransform> outputsWithColorSpaceTransforms;

        Output* output = frag->getOutput();
        const bool isColorOutput = output->getType() == RtType::COLOR3 || output->getType() == RtType::COLOR4;

        for (RtAttribute port : node.getInputs())
        {
            const RtToken& sourceSpace = port.getColorSpace();
            if (sourceSpace != EMPTY_TOKEN && sourceSpace != targetSpace)
            {
                const RtToken portType = port.getType();
                if (portType == RtType::FILENAME && isColorOutput)
                {
                    outputsWithColorSpaceTransforms[output] = ColorSpaceTransform(output->getType(), sourceSpace, targetSpace);
                }
                else if (portType == RtType::COLOR3 || portType == RtType::COLOR4)
                {
                    RtInput inputPort = port.asA<RtInput>();
                    RtOutput upstream = inputPort.getConnection();
                    if (!inputPort.getConnection())
                    {
                        Input* input = frag->getInput(port.getName());
                        inputsWithColorSpaceTransforms[input] = ColorSpaceTransform(portType, sourceSpace, targetSpace);
                    }
                }
            }
        }

        if (inputsWithColorSpaceTransforms.size() || outputsWithColorSpaceTransforms.size())
        {
            // Create a container fragment graph to host the sub-fragments.
            FragmentPtr containerFrag = FragmentGraph::create(frag->getName());
            FragmentGraph* container = containerFrag->asA<FragmentGraph>();
            for (size_t i = 0; i < frag->numInputs(); ++i)
            {
                const Input* port = frag->getInput(i);
                container->createInput(port->getName(), port->getType());
            }
            for (size_t i = 0; i < frag->numOutputs(); ++i)
            {
                const Output* port = frag->getOutput(i);
                container->createOutput(port->getName(), port->getType());
            }

            // Move the fragment from its parent to the container.
            FragmentGraph* parent = frag->getParent();
            if (parent)
            {
                parent->removeFragment(frag->getName());
                parent->addFragment(containerFrag);
            }
            container->addFragment(frag);

            // Connect the fragment's inputs to the container sockets and add
            // sub-fragments inbetween for inputs that use color management.
            for (size_t i = 0; i < frag->numInputs(); ++i)
            {
                Input* input = frag->getInput(i);
                auto it = inputsWithColorSpaceTransforms.find(input);
                if (it != inputsWithColorSpaceTransforms.end())
                {
                    FragmentPtr transformFragment = cms->createFragment(it->second);
                    container->addFragment(transformFragment);
                    container->connect(container->getInputSocket(i), transformFragment->getInput(0));
                    container->connect(transformFragment->getOutput(), input);
                }
                else
                {
                    container->connect(container->getInputSocket(i), frag->getInput(i));
                }
            }

            // Connect the fragment's outputs to the container sockets and add
            // sub-fragments inbetween for outputs that use color management.
            for (size_t i = 0; i < frag->numOutputs(); ++i)
            {
                Output* port = frag->getOutput(i);
                auto it = outputsWithColorSpaceTransforms.find(port);
                if (it != outputsWithColorSpaceTransforms.end())
                {
                    FragmentPtr transformFragment = cms->createFragment(it->second);
                    container->addFragment(transformFragment);
                    container->connect(port, transformFragment->getInput(0));
                    container->connect(transformFragment->getOutput(), container->getOutputSocket(i));
                }
                else
                {
                    container->connect(port, container->getOutputSocket(i));
                }
            }

            // Finalize creation.
            container->finalize(_context);

            // The container is our new fragment.
            frag = containerFrag;
        }
    }

    return frag;
}

uint32_t FragmentGenerator::getClassificationMask(const RtNodeDef& nodedef) const
{
    const RtToken nodeType = nodedef.getNode();
    const RtToken nodeGroup = nodedef.getNodeGroup();

    // Defaulting to texture node
    uint32_t mask = FragmentClassification::TEXTURE;

    // First, check for specific output types
    const RtToken primaryOutputType = nodedef.getOutput().getType();
    if (primaryOutputType == RtType::SURFACESHADER)
    {
        mask = FragmentClassification::SURFACE | FragmentClassification::SHADER;
    }
    else if (primaryOutputType == RtType::LIGHTSHADER)
    {
        mask = FragmentClassification::LIGHT | FragmentClassification::SHADER;
    }
    else if (primaryOutputType == RtType::BSDF)
    {
        mask = FragmentClassification::BSDF | FragmentClassification::CLOSURE;

        // Add additional classifications if the BSDF is restricted to
        // only reflection or transmission

        const RtTypedValue* bsdf = nodedef.getMetadata(Tokens::BSDF, RtType::TOKEN);
        if (bsdf)
        {
            const RtToken val = bsdf->getValue().asToken();
            if (val == Tokens::BSDF_R)
            {
                mask |= FragmentClassification::BSDF_R;
            }
            else if (val == Tokens::BSDF_T)
            {
                mask |= FragmentClassification::BSDF_T;
            }
        }

        // Check for specific nodedefs.
        static const RtToken ND_layer_bsdf("ND_layer_bsdf");
        static const RtToken ND_thin_film_bsdf("ND_thin_film_bsdf");
        if (nodedef.getName() == ND_layer_bsdf)
        {
            mask |= FragmentClassification::LAYER;
        }
        // Check specifically for the thin-film node
        else if (nodedef.getName() == ND_thin_film_bsdf)
        {
            mask |= FragmentClassification::THINFILM;
        }
    }
    else if (primaryOutputType == RtType::EDF)
    {
        mask = FragmentClassification::EDF | FragmentClassification::CLOSURE;
    }
    else if (primaryOutputType == RtType::VDF)
    {
        mask = FragmentClassification::VDF | FragmentClassification::CLOSURE;
    }
    // Second, check for specific nodes types
    else if (nodeType == Tokens::CONSTANT)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONSTANT;
    }
    else if (nodeType == Tokens::COMPARE)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONDITIONAL | FragmentClassification::IFELSE;
    }
    else if (nodeType == Tokens::SWITCH)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::CONDITIONAL | FragmentClassification::SWITCH;
    }
    // Third, check for file texture classification by group name
    else if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::TEXTURE3D_GROUPNAME)
    {
        mask = FragmentClassification::TEXTURE | FragmentClassification::FILETEXTURE;
    }

    // Add in group classification
    if (nodeGroup == Tokens::TEXTURE2D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL2D_GROUPNAME)
    {
        mask |= FragmentClassification::SAMPLE2D;
    }
    else if (nodeGroup == Tokens::TEXTURE3D_GROUPNAME || nodeGroup == Tokens::PROCEDURAL3D_GROUPNAME)
    {
        mask |= FragmentClassification::SAMPLE3D;
    }

    return mask;
}

void FragmentGenerator::registerFragmentClass(const RtToken& className, FragmentCreatorFunction creator)
{
    // NOTE: Allow overwriting any previously registered creators
    // in order for derived generators to override a class creator function.
    _creatorFunctions[className] = creator;
}

} // namespace Codegen
} // namespace MaterialX
