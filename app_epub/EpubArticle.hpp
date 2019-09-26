#pragma once

#ifndef CXXEPUB_EPUBARTICLE_HPP
#define CXXEPUB_EPUBARTICLE_HPP

#include "../Kernel.hpp"

#include "EpubBasic.hpp"

namespace app::epub
{

class Article : public Basic
{
private:


public:
    using Basic::Basic;

    Article() :
        mPath{},
        mSymbol{}
    {}

    ~Article() override = default;

    Article &SetFilePath(ArticlePath &value)
    {
        this->mPath = value;
        return (*this);
    }

    Article &SetDirctoryPath(ArticlePath &value)
    {
        this->mDirctory = value;
        return (*this);
    }

    Article &SetSplitSymbol(ArticleSplitSymbol &v)
    {
        this->mSymbol = v;
        return (*this);
    }

    Article &Parse()
    {
        if ((this->mPath.IsExist()) || (this->mDirctory.IsDirctory()))
            return (*this);

        return (*this);
    }

private:
    ArticlePath mPath;
    ArticlePath mDirctory;
    ArticleSplitSymbol mSymbol;
};

}

#endif //CXXEPUB_EPUBARTICLE_HPP
