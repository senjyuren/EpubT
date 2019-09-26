#pragma once

#ifndef CXXEPUB_EPUBCOMMANDS_HPP
#define CXXEPUB_EPUBCOMMANDS_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsLog.hpp"
#include "../app_utils/UtilsJString.hpp"

#include "EpubBasic.hpp"
#include "EpubCommandsBasic.hpp"
#include "EpubCommandsDefine.hpp"
#include "EpubCommandsCollection.hpp"

namespace app::epub
{

class Commands : public Basic
{
public:
    using Basic::Basic;

    Commands() :
        mHanlde{}
    {}

    ~Commands() override = default;

    Commands &SetCommands(Jchar **v, Jsize vLen) override
    {
        Basic::SetCommands(v, vLen);
        return (*this);
    }

    void Run()
    {
        this->mHanlde.Register(&this->mCmdHelp);
        this->mHanlde.Register(&this->mCmdXhtml);
        this->mHanlde.Register(&this->mCmdSplit);

        for (Jint i = 0; i < this->mCommandsSize; ++i)
            this->mHanlde.SetKeyAndValue(this->mCommands[i].GetKey(), this->mCommands[i].GetValue());
        this->mHanlde.Finish();

        this->mHanlde.UnRegister(&this->mCmdHelp);
        this->mHanlde.UnRegister(&this->mCmdXhtml);
        this->mHanlde.UnRegister(&this->mCmdSplit);
    }

private:
    CommandsHandle mHanlde;

    CommandsHelp mCmdHelp;
    CommandsXhtml mCmdXhtml;
    CommandsSplit mCmdSplit;
};

}

#endif //CXXEPUB_EPUBCOMMANDS_HPP
