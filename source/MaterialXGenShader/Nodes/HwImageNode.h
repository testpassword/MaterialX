//
// TM & (c) 2019 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_HWIMAGECODENODE_H
#define MATERIALX_HWIMAGECODENODE_H

#include <MaterialXGenShader/Nodes/HwSourceCodeNode.h>

namespace MaterialX
{

/// Extending the HwSourceCodeNode with requirements for image nodes.
class MX_GENSHADER_API HwImageNode : public HwSourceCodeNode
{
public:
    static ShaderNodeImplPtr create();

    void addInputs(ShaderNode& node, GenContext& context) const override;
    void setValues(const Node& node, ShaderNode& shaderNode, GenContext& context) const override;

    // Aguments which may be added as part of the signature for image nodes
    // to allow for additional texture coordinate transformations.
    // Currently these are added to support UDIM texture atlas lookup.
    static string UV_SCALE;
    static string UV_OFFSET;
};

} // namespace MaterialX

#endif
