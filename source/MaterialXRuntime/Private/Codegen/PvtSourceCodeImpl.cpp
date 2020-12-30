//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXRuntime/Private/Codegen/PvtSourceCodeImpl.h>
#include <MaterialXRuntime/Codegen/RtCodeGenerator.h>
#include <MaterialXRuntime/RtApi.h>

#include <MaterialXFormat/Util.h>

namespace MaterialX
{

PvtSourceCodeImpl::PvtSourceCodeImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent)
    : PvtCodegenImpl(typeInfo, name, parent)
{
}

void PvtSourceCodeImpl::setFile(const string& file)
{
    _file = file;

    const FilePath path = RtApi::get().getSearchPath().find(file);
    string source = readFile(path);
    if (source.empty())
    {
        throw ExceptionShaderGenError("Failed to get source code from file '" + path.asString() +
            "' used by implementation '" + getName().str() + "'");
    }

    setSourceCode(source);
}

void PvtSourceCodeImpl::emitFunctionDefinition(const RtNode& /*node*/, RtCodegenContext& /*context*/, RtFragment& frag) const
{
    // Emit function definition for non-inlined functions
    if (!_function.empty() && !_source.empty())
    {
        frag.addBlock(_source);
        frag.newLine();
    }
}

void PvtSourceCodeImpl::emitFunctionCall(const RtNode& /*node*/, RtCodegenContext& /*context*/, RtFragment& frag) const
{
    frag.addLine("You can call me Al");
}

}
