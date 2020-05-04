//
// TM & (c) 2020 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#ifndef MATERIALX_PVTRENAMEPRIMCMD_H
#define MATERIALX_PVTRENAMEPRIMCMD_H

#include <MaterialXRuntime/Private/PvtCommandEngine.h>

#include <MaterialXRuntime/RtStage.h>
#include <MaterialXRuntime/RtPath.h>

namespace MaterialX
{

class PvtRenamePrimCmd : public PvtCommand
{
public:
    PvtRenamePrimCmd(RtStagePtr stage, const RtPath& path, const RtToken& newName) :
        _stage(stage),
        _path(path),
        _newName(newName)
    {}

    static PvtCommandPtr create(RtStagePtr stage, const RtPath& path, const RtToken& newName);

    void execute(RtCommandResult& result) override;
    void undo(RtCommandResult& result) override;

private:
    RtStagePtr _stage;
    RtPath _path;
    RtToken _newName;
};

}

#endif