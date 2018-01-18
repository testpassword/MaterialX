#include <MaterialXShaderGen/Implementations/Switch.h>
#include <MaterialXShaderGen/Shader.h>
#include <MaterialXShaderGen/ShaderGenerator.h>

namespace MaterialX
{

const vector<string> Switch::kInputNames = { "in1", "in2", "in3", "in4", "in5" };

SgImplementationPtr Switch::creator()
{
    return std::make_shared<Switch>();
}

void Switch::emitFunctionCall(const SgNode& node, ShaderGenerator& shadergen, Shader& shader)
{
    // Declare the output variable
    shader.beginLine();
    shadergen.emitOutput(node.getOutput(), true, shader);
    shader.endLine();

    static const vector<string> inputNames = { "in1", "in2", "in3", "in4", "in5" };
    const SgInput* which = node.getInput("which");

    // Process the five branches of the switch node
    for (int branch = 0; branch < 5; ++branch)
    {
        const SgInput* input = node.getInput(inputNames[branch]);

        shader.beginLine();
        if (branch > 0)
        {
            shader.addStr("else ");
        }
        if (branch < 5)
        {
            shader.addStr("if ("); shadergen.emitInput(which, shader); shader.addStr(" < "); shader.addValue(float(branch+1));  shader.addStr(")");
        }
        shader.endLine(false);

        shader.beginScope();

        // Emit nodes that are ONLY needed in this scope
        for (SgNode* otherNode : shader.getNodeGraph()->getNodes())
        {
            const SgNode::ScopeInfo& scope = otherNode->getScopeInfo();
            if (scope.conditionalNode == &node && scope.usedByBranch(branch))
            {
                shader.addFunctionCall(otherNode, shadergen);
            }
        }

        shader.beginLine();
        shadergen.emitOutput(node.getOutput(), false, shader);
        shader.addStr(" = ");
        shadergen.emitInput(input, shader);
        shader.endLine();

        shader.endScope();
    }
}

} // namespace MaterialX