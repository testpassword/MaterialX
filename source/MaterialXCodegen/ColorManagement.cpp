//
// TM & (c) 2017 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXCodegen/ColorManagement.h>

#include <MaterialXRuntime/RtApi.h>
#include <MaterialXRuntime/RtTypeDef.h>
#include <MaterialXRuntime/Tokens.h>

#include <MaterialXFormat/Util.h>

namespace MaterialX 
{
namespace Codegen
{

ColorSpaceTransform::ColorSpaceTransform()
{
}

ColorSpaceTransform::ColorSpaceTransform(const RtToken& type_, const RtToken& sourceSpace_, const RtToken& targetSpace_) :
    type(type_),
    sourceSpace(sourceSpace_),
    targetSpace(targetSpace_)
{
    if (type != RtType::COLOR3 && type != RtType::COLOR4)
    {
        throw ExceptionRuntimeError("Color space transform can only be a color3 or color4.");
    }
}


DefaultColorManagementSystem::DefaultColorManagementSystem(const RtToken& target) :
    _target(target)
{}

ColorManagementSystemPtr DefaultColorManagementSystem::create(const RtToken& target)
{
    return ColorManagementSystemPtr(new DefaultColorManagementSystem(target));
}

const RtToken& DefaultColorManagementSystem::getName() const
{
    static const RtToken NAME("default_cms");
    return NAME;
}

FragmentPtr DefaultColorManagementSystem::createFragment(const ColorSpaceTransform& transform) const
{
    const RtToken fragmentName("IM_" + transform.sourceSpace.str() + "_to_" + 
        transform.targetSpace.str() + "_" + transform.type.str() + "_" + _target.str());
    auto it = _fragments.find(fragmentName);
    return it != _fragments.end() ? it->second->clone() : nullptr;
}

void DefaultColorManagementSystem::loadImplementations(const FilePath& file)
{
    DocumentPtr doc = createDocument();
    readFromXmlFile(doc, file, RtApi::get().getSearchPath());

    for (auto impl : doc->getImplementations())
    {
        const string& target = impl->getTarget();
        if (target == _target.str())
        {
            const string& name = impl->getName();
            const string& function = impl->getFunction();
            const string& sourceFile = impl->getFile();

            const FilePath path = RtApi::get().getSearchPath().find(sourceFile);
            string content = readFile(path);
            if (content.empty())
            {
                throw ExceptionRuntimeError("Failed to get source code from file '" + path.asString() +
                    "' used by implementation '" + name + "'");
            }
            // Remove newline if it's an inline expression.
            if (function.empty())
            {
                content.erase(std::remove(content.begin(), content.end(), '\n'), content.end());
            }

            // Take ownership of the source code so we can reference it
            // in our source fragment below.
            _sourceCode.push_back(content);

            // Find the data type from the impl name.
            RtToken type = EMPTY_TOKEN;
            size_t n = name.rfind(target);
            if (n > 6 && n < name.size())
            {
                type = RtToken(name.substr(n - 7, 6));
            }
            if (!(type == RtType::COLOR3 || type==RtType::COLOR4))
            {
                throw ExceptionRuntimeError("Implementation '" + name + "' is not a valid color transform implementation of color type");
            }

            FragmentPtr frag = SourceFragment::create(RtToken(name));
            SourceFragment* sourceFragment = frag->asA<SourceFragment>();
            sourceFragment->createInput(type, Tokens::IN);
            sourceFragment->createOutput(type, Tokens::OUT);
            sourceFragment->setSourceCode(&_sourceCode.back());
            sourceFragment->setFunctionName(RtToken(function));

            _fragments[frag->getName()] = frag;
        }
    }
}

} // namespace Codegen
} // namespace MaterialX
