#pragma once

#ifndef CXXEPUB_EPUBARTICLEBASIC_HPP
#define CXXEPUB_EPUBARTICLEBASIC_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsJString.hpp"

#include <sys/types.h>
#include <sys/stat.h>

namespace app::epub
{

enum class ArticleType
{
    NONE,
    TITLE,
    PART,
};

class ArticleTitle : public app::utils::JString<128>
{
public:
    using JString::JString;

    ArticleTitle() = default;

    ~ArticleTitle() override = default;
};

class ArticlePart : public app::utils::JString<4096>
{
public:
    using JString::JString;

    ArticlePart() = default;

    ~ArticlePart() override = default;
};

class ArticleSplitSymbol : public app::utils::JString<32>
{
public:
    using JString::JString;

    ArticleSplitSymbol() = default;

    ~ArticleSplitSymbol() override = default;
};

class ArticlePath : public app::utils::JString<256>
{
public:
    using JString::JString;

    ArticlePath():
        mFileStat{}
    {}

    ~ArticlePath() override = default;

    Jbool IsExist()
    {
        if (this->IsEmpty())
            return false;
        return (_access(reinterpret_cast<Jchar *>(this->mCache), 0) == 0);
    }

    Jbool IsDirctory()
    {
        stat(reinterpret_cast<Jchar *>(this->mCache),&this->mFileStat);
        return (this->mFileStat.st_mode & _S_IFDIR);
    }

private:
    struct stat mFileStat;
};

}

#endif //CXXEPUB_EPUBARTICLEBASIC_HPP
