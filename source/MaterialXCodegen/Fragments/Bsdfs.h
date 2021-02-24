//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_CODEGEN_BSDFS_H
#define MATERIALX_CODEGEN_BSDFS_H

/// @file Bsdfs.h
/// Bsdf fragments.

#include <MaterialXCodegen/Library.h>
#include <MaterialXCodegen/Fragment.h>

namespace MaterialX
{
namespace Codegen
{

/// @class LayerFragment
/// A fragment for the layer node.
class LayerFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(LayerFragment)

public:
    void emitFunctionCall(const Context& context, SourceCode& result) const override;

    /// Adding a base BSDF input to a fragment.
    static void addBsdfLayerSupport(Fragment& frag);

    /// Perform setup of the layer stack in the given graph
    /// to make it ready for compilation.
    /// To be called during fragment graphs finalization.
    static void setupLayerStack(FragmentGraph& graph);
};

/// @class ThinFilmBsdfFragment
/// A fragment for the thin-film bsdf.
class ThinFilmBsdfFragment : public Fragment
{
    DECLARE_FRAGMENT_CLASS(ThinFilmBsdfFragment)

public:
    void emitFunctionCall(const Context& context, SourceCode& result) const override;

    void finalize(const Context& context) override;

    /// Adding inputs for thin-film supportto a BSDF fragment.
    static void addThinFilmSupport(Fragment& frag);
};

/// @class LayerableBsdfFragment
/// A fragment for a BSDF that supports vertical layering.
class LayerableBsdfFragment : public SourceFragment
{
    DECLARE_FRAGMENT_CLASS(LayerableBsdfFragment)

public:
    void copy(const Fragment& other) override;

    void emitFunctionCall(const Context& context, SourceCode& result) const override;

    void finalize(const Context& context) override;

    bool enableFunctionCall() const
    {
        return _enableFunctionCall;
    }

    void setEnableFunctionCall(bool enable)
    {
        _enableFunctionCall = enable;
    }

protected:
    bool _enableFunctionCall = true;
};

/// @class DielectricBsdfFragment
/// A fragment for a dielectriv BSDF that supports both
/// vertical layering and thin-film.
class DielectricBsdfFragment : public LayerableBsdfFragment
{
    DECLARE_FRAGMENT_CLASS(DielectricBsdfFragment)

public:
    void finalize(const Context& context) override;
};

} // namespace Codegen
} // namespace MaterialX

#endif
