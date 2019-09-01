#pragma once

#ifndef CXXEPUB_EPUBCOMMANDS_HPP
#define CXXEPUB_EPUBCOMMANDS_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsLog.hpp"
#include "../app_utils/UtilsJString.hpp"

#include "EpubBasic.hpp"

namespace app::epub
{

class Commands : public Basic
{
protected:
    template<Jsize _size>
    using JString = app::utils::JString<_size>;

    constexpr static Jint COMMANDS_KEY_SIZE = 64;
    constexpr static Jint COMMANDS_VALUE_SIZE = 64;

    using CommandsResults = void (*)(void *p, JString<COMMANDS_KEY_SIZE> &key, JString<COMMANDS_VALUE_SIZE> &value);
public:
    Commands() :
        mResultP{},
        mResultsListen{}
    {}

    ~Commands() = default;

    Commands &SetResultsListener(void *p, CommandsResults listen)
    {
        this->mResultP = p;
        this->mResultsListen = listen;
        return (*this);
    }

    void Run()
    {
        Jint i = 0;

        if (this->mResultsListen == nullptr)
            return;
        for (i = 0; i < this->mCommandsSize; ++i)
            this->mResultsListen(this->mResultP, this->mCommands[i].key, this->mCommands[i].value);
    }

private:
    void *mResultP;
    CommandsResults mResultsListen;
};

}

#endif //CXXEPUB_EPUBCOMMANDS_HPP
