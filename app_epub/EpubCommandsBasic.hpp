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

    ~CommandKey() override = default;
};

class CommandValue : public app::utils::JString<64>
{
public:
    using JString::JString;

    CommandValue() = default;

    ~CommandValue() override = default;
};

class CommandsPair
{
public:
    CommandsPair() :
        mKey{},
        mValue{}
    {}

    virtual ~CommandsPair() = default;

    CommandsPair &SetKey(const CommandKey &key)
    {
        this->mKey = key;
        return (*this);
    }

    CommandsPair &SetValue(const CommandValue &value)
    {
        this->mValue = value;
        return (*this);
    }

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
