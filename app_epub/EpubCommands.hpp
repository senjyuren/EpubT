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
    Commands() :
        mHanlde{}
    {}

    ~Commands() = default;

    Commands &SetCommands(Jchar **v, Jsize vLen) override
    {
        Basic::SetCommands(v, vLen);
        return (*this);
    }

    void Run()
    {
        auto &&collection = new CommandsHelp();
        auto &&xhtml = new CommandsXhtml();
        auto &&split = new CommandsSplit();

        this->mHanlde.Register(collection);
        this->mHanlde.Register(xhtml);
        this->mHanlde.Register(split);

        for (Jint i = 0; i < this->mCommandsSize; ++i)
            this->mHanlde.SetKeyAndValue(this->mCommands[i].GetKey(), this->mCommands[i].GetValue());
        this->mHanlde.Finish();

        this->mHanlde.UnRegister(collection);
        this->mHanlde.UnRegister(xhtml);
        this->mHanlde.UnRegister(split);
    }

private:
    CommandsHandle mHanlde;
};

}

#endif //CXXEPUB_EPUBCOMMANDS_HPP
