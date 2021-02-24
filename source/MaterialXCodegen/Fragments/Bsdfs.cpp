//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#include <MaterialXCodegen/Fragments/Bsdfs.h>
#include <MaterialXCodegen/Context.h>
#include <MaterialXCodegen/FragmentCompiler.h>

#include <MaterialXRuntime/Tokens.h>

namespace MaterialX
{
namespace Codegen
{

namespace
{
    const RtToken THIN_FILM_INPUT("tf");
}

DEFINE_FRAGMENT_CLASS(LayerFragment, Fragment)

void LayerFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();

    Input* top = getInput(0);
    Input* base = getInput(1);
    Output* output = getOutput();
    if (!top->isConnected())
    {
        // No top layer, just emit default value.
        result.beginLine();
        compiler.declareVariable(*output, true, result);
        result.endLine();
        return;
    }

    Fragment* topBsdf = top->getConnection()->getParent();

    // Layerable nodes require a BSDF input named "base"
    Input* topBsdfInput = topBsdf->getInput(Tokens::BASE);
    if (!topBsdfInput || topBsdfInput->getType() != RtType::BSDF || !topBsdf->isA<LayerableBsdfFragment>())
    {
        throw ExceptionRuntimeError("Node connected as top layer '" + topBsdf->getName().str() + "' is not a layerable BSDF");
    }

    LayerableBsdfFragment* layerableTopBsdf = topBsdf->asA<LayerableBsdfFragment>();

    Output* topBsdfOutput = layerableTopBsdf->getOutput();

    // Save state so we can restore it below
    Output* topBsdfInputOldConnection = topBsdfInput->getConnection();
    const RtToken topBsdfOutputOldVariable = topBsdfOutput->getVariable();
    bool enabled = layerableTopBsdf->enableFunctionCall();

    // Change the state so we emit the top layer function 
    // with base layer connection and output variable name
    // from the layer node itself.
    Output* baseBsdfOutput = base->getConnection();
    if (baseBsdfOutput)
    {
        _parent->connect(baseBsdfOutput, topBsdfInput);
    }
    else
    {
        _parent->disconnect(topBsdfInput->getConnection(), topBsdfInput);
    }
    topBsdfOutput->setVariable(output->getVariable());

    // Emit the function call.
    layerableTopBsdf->setEnableFunctionCall(true);
    layerableTopBsdf->emitFunctionCall(context, result);

    // Restore state.
    layerableTopBsdf->setEnableFunctionCall(enabled);
    if (topBsdfInputOldConnection)
    {
        _parent->connect(topBsdfInputOldConnection, topBsdfInput);
    }
    topBsdfOutput->setVariable(topBsdfOutputOldVariable);
}

void LayerFragment::addBsdfLayerSupport(Fragment& frag)
{
    frag.createInput(Tokens::BASE, RtType::BSDF);
}


void LayerFragment::setupLayerStack(FragmentGraph& graph)
{
    // First check if any layer operators are used.
    bool layerOperatorUsed = false;
    for (size_t i = 0; i < graph.numFragments(); ++i)
    {
        Fragment* frag = graph.getFragment(i);
        if (frag->hasClassification(FragmentClassification::LAYER))
        {
            layerOperatorUsed = true;
            break;
        }
    }

    if (!layerOperatorUsed)
    {
        // No vertical layering is used 
        // so no work to do here.
        return;
    }

    // Check if any BSDF is strictly used as top layer in vertical layering.
    // If so we can disable its function call since the layering node will
    // emit this for each layering instance used.
    for (size_t i = 0; i < graph.numFragments(); ++i)
    {
        Fragment* frag = graph.getFragment(i);
        if (frag->isA<LayerableBsdfFragment>())
        {
            LayerableBsdfFragment* layerableFragment = frag->asA<LayerableBsdfFragment>();
            bool enable = false;
            const Output* output = layerableFragment->getOutput();
            for (const Input* downstreamInput : output->getConnections())
            {
                if (!(downstreamInput->getParent()->hasClassification(FragmentClassification::LAYER) &&
                    downstreamInput->getName() == Tokens::TOP))
                {
                    // This is not a connection to a layer operator "top" input.
                    // So we should keep this function call enabled.
                    enable = true;
                    break;
                }
            }

            layerableFragment->setEnableFunctionCall(enable);
        }
    }

    // Find all thin-film nodes and reconnect them to the 'thinfilm' input
    // on BSDF nodes layered underneath.
    for (size_t i = 0; i < graph.numFragments(); ++i)
    {
        Fragment* frag = graph.getFragment(i);

        if (frag->hasClassification(FragmentClassification::THINFILM))
        {
            Output* output = frag->getOutput();

            // Change type to our 'thinfilm' data type since this
            // is not a BSDF in our default implementation.
            output->setType(RtType::THINFILM);

            // Find vertical layering fragments connected to this thinfilm.
            vector<Fragment*> layerFragments;
            for (Input* dest : output->getConnections())
            {
                Fragment* downstreamFragment = dest->getParent();

                // Make sure the connection is valid.
                if (!(downstreamFragment->hasClassification(FragmentClassification::LAYER) && dest->getName() == Tokens::TOP))
                {
                    throw ExceptionRuntimeError("Invalid connection from '" + frag->getName().str() + "' to '" + downstreamFragment->getName().str() +
                        "." + dest->getName().str() + "'. " + "Thin-film can only be connected to a <layer> operator's top input.");
                }

                layerFragments.push_back(downstreamFragment);
            }

            // Remove all connections from the thin-film node.
            graph.disconnect(output);

            for (Fragment* layerFragment : layerFragments)
            {
                Input* base = layerFragment->getInput(Tokens::BASE);
                if (base && base->getConnection())
                {
                    Fragment* bsdf = base->getConnection()->getParent();

                    // Save the output to use for bypassing the layer node below.
                    Output* bypassOutput = bsdf->getOutput();

                    // Handle the case where the bsdf below is an additional layer operator.
                    if (bsdf->hasClassification(FragmentClassification::LAYER))
                    {
                        // In this case get the top bsdf since this is where microfacet bsdfs
                        // are placed. Only one such extra layer indirection is supported.
                        // We need this in order to support having thin-film applied to a
                        // microfacet bsdf that itself is layered on top of a substrate.
                        Input* top = bsdf->getInput(Tokens::TOP);
                        bsdf = top && top->getConnection() ? top->getConnection()->getParent() : nullptr;
                    }

                    Input* bsdfInput = bsdf ? bsdf->getInput(THIN_FILM_INPUT) : nullptr;
                    if (!bsdfInput)
                    {
                        throw ExceptionRuntimeError("No BSDF node supporting thin-film was found for '" + frag->getName().str() + "'");
                    }

                    // Connect the thinfilm node to the bsdf input.
                    graph.connect(output, bsdfInput);

                    // Bypass the layer node since thin-film is now setup on the bsdf.
                    // Iterate a copy of the connection set since the original set will
                    // change when breaking connections.
                    graph.disconnect(base);
                    ConnectionSet downstreamConnections = layerFragment->getOutput()->getConnections();
                    for (Input* downstream : downstreamConnections)
                    {
                        graph.disconnect(downstream);
                        graph.connect(bypassOutput, downstream);
                    }
                }
            }
        }
    }
}


DEFINE_FRAGMENT_CLASS(ThinFilmBsdfFragment, Fragment)

void ThinFilmBsdfFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    const FragmentCompiler& compiler = context.getCompiler();
    const Syntax& syntax = context.getSyntax();

    const Input* thickness = getInput(Tokens::THICKNESS);
    const Input* ior       = getInput(Tokens::IOR);
    const Output* output   = getOutput();

    result.addLine(syntax.getTypeName(RtType::THINFILM) + " " + output->getVariable().str());
    result.addLine(output->getVariable().str() + "." + Tokens::THICKNESS.str() + " = " + compiler.getResult(*thickness));
    result.addLine(output->getVariable().str() + "." + Tokens::IOR.str() + " = " + compiler.getResult(*ior));
}

void ThinFilmBsdfFragment::finalize(const Context&)
{
    LayerFragment::addBsdfLayerSupport(*this);
}

void ThinFilmBsdfFragment::addThinFilmSupport(Fragment& frag)
{
    frag.createInput(THIN_FILM_INPUT, RtType::THINFILM);
}


DEFINE_FRAGMENT_CLASS(LayerableBsdfFragment, SourceFragment)

void LayerableBsdfFragment::copy(const Fragment& other)
{
    SourceFragment::copy(other);
    const LayerableBsdfFragment* otherFragment = other.asA<const LayerableBsdfFragment>();
    _enableFunctionCall = otherFragment->_enableFunctionCall;
}

void LayerableBsdfFragment::emitFunctionCall(const Context& context, SourceCode& result) const
{
    if (_enableFunctionCall)
    {
        SourceFragment::emitFunctionCall(context, result);
    }
}

void LayerableBsdfFragment::finalize(const Context&)
{
    LayerFragment::addBsdfLayerSupport(*this);
}

DEFINE_FRAGMENT_CLASS(DielectricBsdfFragment, LayerableBsdfFragment)

void DielectricBsdfFragment::finalize(const Context& context)
{
    LayerableBsdfFragment::finalize(context);
    ThinFilmBsdfFragment::addThinFilmSupport(*this);
}

} // namespace Codegen
} // namespace MaterialX
