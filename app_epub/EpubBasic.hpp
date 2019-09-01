#pragma once

#ifndef CXXEPUB_EPUBBASIC_HPP
#define CXXEPUB_EPUBBASIC_HPP

#include "../Kernel.hpp"

#include "EpubCommandsBasic.hpp"

namespace app::epub
{

class Basic
{
protected:
    constexpr static Jsize EPUBBASIC_COMMANDS_SIZE = 1024;

    typedef struct BasicCommands
    {
        CommandKey key;
        CommandValue value;

        BasicCommands() :
            key{},
            value{}
        {}
    } BasicCommands;

    constexpr static Jchar EPUBBASIC_COMMANDS_KEY_MARK = '-';
    constexpr static Jchar EPUBBASIC_COMMANDS_VALUE_MARK = '=';

public:
    Basic() :
        mCommandsSize{},
        mCommands{}
    {}

    ~Basic() = default;

    void SetCommands(Jchar **v, Jsize vLen)
    {
        Jint i = 0;

        if ((v == nullptr) || (vLen < 2))
            return;

        for (i = 1; i < static_cast<Jint>(vLen); ++i)
        {
            if (v[i] == nullptr)
                continue;

            auto &&commandLen = static_cast<Jint>(strlen(v[i]));
            if (v[i][0] != EPUBBASIC_COMMANDS_KEY_MARK)
                continue;

            auto &&positionPtr = reinterpret_cast<Jchar *>(memchr(&v[i][1], EPUBBASIC_COMMANDS_VALUE_MARK, commandLen));
            if (positionPtr == nullptr)
                continue;

            auto &&position = positionPtr - &v[i][0];
            v[i][position] = 0x00;
            ++position;
            this->mCommands[this->mCommandsSize].key = &v[i][1];
            this->mCommands[this->mCommandsSize].value = &v[i][position];
            ++this->mCommandsSize;
        }
    }

    void Clean()
    {
        this->mCommandsSize = 0;
        memset(&this->mCommands, 0, sizeof(this->mCommands));
    }

protected:
    Jint mCommandsSize;
    BasicCommands mCommands[EPUBBASIC_COMMANDS_SIZE];
};

}

#endif //CXXEPUB_EPUBBASIC_HPP
