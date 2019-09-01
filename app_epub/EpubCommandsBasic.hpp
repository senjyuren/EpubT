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

}

#endif //CXXEPUB_EPUBCOMMANDSBASIC_HPP
