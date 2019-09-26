#pragma once

#ifndef CXXEPUB_EPUBBASIC_HPP
#define CXXEPUB_EPUBBASIC_HPP

#include "../Kernel.hpp"

#include "EpubArticleBasic.hpp"
#include "EpubArticleCollection.hpp"
#include "EpubCommandsBasic.hpp"

namespace app::epub
{

class Basic
{
private:
    constexpr static Jsize EPUBBASIC_COMMANDS_SIZE = 1024;

    constexpr static Jchar EPUBBASIC_COMMANDS_KEY_MARK = '-';
    constexpr static Jchar EPUBBASIC_COMMANDS_VALUE_MARK = '=';

public:
    Basic() :
        mCommandsSize{},
        mCommands{},
        mArticleWriter{},
        mArticlePartStorage{},
        mArticleTitleStorage{}
    {}

    virtual ~Basic() = default;

    virtual Basic &SetCommands(Jchar **v, Jsize vLen)
    {
        Jint i = 0;

        if ((v == nullptr) || (vLen < 2))
            return (*this);

        for (i = 1; i < static_cast<Jint>(vLen); ++i)
        {
            if (v[i] == nullptr)
                continue;

            auto &&commandLen = static_cast<Jint>(strlen(v[i]));
            if (v[i][0] != EPUBBASIC_COMMANDS_KEY_MARK)
                continue;

            auto &&positionPtr = reinterpret_cast<Jchar *>(memchr(&v[i][1], EPUBBASIC_COMMANDS_VALUE_MARK, commandLen));
            if (positionPtr == nullptr)
            {
                this->mCommands[this->mCommandsSize].SetKey(&v[i][1]);
                ++this->mCommandsSize;
                continue;
            }

            auto &&position = positionPtr - &v[i][0];
            v[i][position] = 0x00;
            ++position;
            this->mCommands[this->mCommandsSize].SetKey(&v[i][1])
                .SetValue(&v[i][position]);
            ++this->mCommandsSize;
        }

        return (*this);
    }

    virtual Basic &SetArticleStart()
    {
        this->mArticleWriter.Register(&this->mArticlePartStorage);
        this->mArticleWriter.Register(&this->mArticlePartStorage);
        return (*this);
    }

    virtual Basic &SetArticleTitle(ArticleTitle &v)
    {
        this->mArticleWriter.SetTitle<ArticleType::TITLE>(v);
        return (*this);
    }

    virtual Basic &SetArticlePart(Jbool isNew, ArticlePart &v)
    {
        if (isNew)
            this->mArticleWriter.SetPart<true, ArticleType::PART>(v);
        else
            this->mArticleWriter.SetPart<false, ArticleType::PART>(v);
        return (*this);
    }

    virtual Basic &ArticleFinish()
    {
        this->mArticleWriter.Finish();
        return (*this);
    }

    void Clean()
    {
        this->mCommandsSize = 0;
        this->mArticleWriter.UnRegister(&this->mArticlePartStorage);
        this->mArticleWriter.UnRegister(&this->mArticleTitleStorage);
    }

protected:
    Jint mCommandsSize;
    CommandsPair mCommands[EPUBBASIC_COMMANDS_SIZE];

    ArticleWriter mArticleWriter;
    ArticlePartStorage mArticlePartStorage;
    ArticleTitleStorage mArticleTitleStorage;
};

}

#endif //CXXEPUB_EPUBBASIC_HPP
