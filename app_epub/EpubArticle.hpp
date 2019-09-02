#pragma once

#ifndef CXXEPUB_EPUBARTICLE_HPP
#define CXXEPUB_EPUBARTICLE_HPP

#include "../Kernel.hpp"

#include "EpubBasic.hpp"

namespace app::epub
{

class Article : public Basic
{
public:
    Article() :
        mPath{}
    {}

    ~Article() = default;

    Article &SetFilePath(CommandValue &value)
    {
        this->mPath = value;
        return (*this);
    }

    void Run()
    {
        
    }

private:
    CommandValue mPath;
};

}

#endif //CXXEPUB_EPUBARTICLE_HPP
