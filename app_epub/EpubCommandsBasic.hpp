#pragma once

#ifndef CXXEPUB_EPUBCOMMANDSBASIC_HPP
#define CXXEPUB_EPUBCOMMANDSBASIC_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsJString.hpp"

namespace app::epub
{

class CommandKey : public app::utils::JString<64>
{
public:
    using JString::JString;

    CommandKey() = default;

    ~CommandKey() = default;
};

class CommandValue : public app::utils::JString<64>
{
public:
    using JString::JString;

    CommandValue() = default;

    ~CommandValue() = default;
};

class CommandsPair
{
public:
    CommandsPair() :
        mKey{},
        mValue{}
    {}

    ~CommandsPair() = default;

    CommandsPair(CommandKey key, CommandValue value) :
        mKey{key},
        mValue{value}
    {}

    CommandKey &GetKey()
    {
        return this->mKey;
    }

    CommandValue &GetValue()
    {
        return this->mValue;
    }

private:
    CommandKey mKey;
    CommandValue mValue;
};

}

#endif //CXXEPUB_EPUBCOMMANDSBASIC_HPP
