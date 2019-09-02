#pragma once

#ifndef CXXEPUB_EPUBFACTORY_HPP
#define CXXEPUB_EPUBFACTORY_HPP

#include "../Kernel.hpp"

#include "EpubArticle.hpp"
#include "EpubCommands.hpp"

namespace app::epub
{

enum class FactoryType
{
    ARTICLE,
    COMMANDS,
};

class Factory
{
public:
    static Factory &Instance()
    {
        static Factory *obj = nullptr;
        if (obj == nullptr)
            obj = new Factory();
        return (*obj);
    }

    template<FactoryType _type>
    Basic &Get()
    {
        if (_type == FactoryType::COMMANDS)
        {
            if (this->mCommands == nullptr)
                this->mCommands = new Commands();
            return (*this->mCommands);
        } else if (_type == FactoryType::ARTICLE)
        {
            if (this->mArticle == nullptr)
                this->mArticle = new Article();
            return (*this->mArticle);
        }
    }

private:
    Basic *mArticle;
    Basic *mCommands;

    Factory() :
        mArticle{},
        mCommands{}
    {}

    ~Factory() = default;
};

}

#endif //CXXEPUB_EPUBFACTORY_HPP
