//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved. See LICENSE.txt for license.
//

#ifndef MATERIALX_PVTGRAPHIMPL_H
#define MATERIALX_PVTGRAPHIMPL_H

#include <MaterialXRuntime/Private/Codegen/PvtCodegenImpl.h>

namespace MaterialX
{

// Private implementation of sourcecodeimpl prim.
class PvtSourceCodeImpl : public PvtCodegenImpl
{
public:
    PvtSourceCodeImpl(const RtTypeInfo* typeInfo, const RtToken& name, PvtPrim* parent);

    void setFile(const string& file);

    const string& getFile() const
    {
        return _file;
    }

    void setSourceCode(const string& source)
    {
        _source = source;
    }

    const string& getSourceCode() const
    {
        return _source;
    }

    void setFormat(const RtToken& format)
    {
        _format = format;
    }

    const RtToken& getFormat() const
    {
        return _format;
    }

    void setFunction(const string& function)
    {
        _function = function;
    }

    const string& getFunction() const
    {
        return _function;
    }

    void emitFunctionDefinition(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const override;
    void emitFunctionCall(const RtNode& node, RtCodegenContext& context, RtFragment& frag) const override;

protected:
    string _file;
    string _source;
    string _function;
    RtToken _format;
};

}

#endif
