#pragma once

#ifndef CXXEPUB_EPUBARTICLEBASIC_HPP
#define CXXEPUB_EPUBARTICLEBASIC_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsJString.hpp"

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
    ArticleTitle() = default;

    ~ArticleTitle() = default;
};

class ArticlePart : public app::utils::JString<4096>
{
public:
    ArticlePart() = default;

    ~ArticlePart() = default;
};

}

#endif //CXXEPUB_EPUBARTICLEBASIC_HPP
