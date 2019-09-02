#pragma once

#ifndef CXXEPUB_EPUBARTICLECOLLECTION_HPP
#define CXXEPUB_EPUBARTICLECOLLECTION_HPP

#include "../Kernel.hpp"

#include "EpubArticleBasic.hpp"

namespace app::epub
{

class ArticleObserver;

class ArticleSubject
{
public:
    ArticleSubject() = default;

    virtual ~ArticleSubject() = default;

    virtual void Register(ArticleObserver *v) = 0;

    virtual void UnRegister(ArticleObserver *v) = 0;

    virtual void Notify() = 0;

    virtual void Done() = 0;

    virtual void Clean() = 0;
};

class ArticleObserver
{
public:
    ArticleObserver() = default;

    virtual  ~ArticleObserver() = default;

    virtual void UpdateTitle(ArticleTitle &)
    {};

    virtual void UpdatePart(Jbool, ArticlePart &)
    {};

    virtual void Finish() = 0;

    virtual void Clean() = 0;
};

class ArticleWriter : public ArticleSubject
{
private:
    constexpr static Jsize ArticleWriter_DEFAULT_SIZE = 40960;

public:
    ArticleWriter() :
        isNew{},
        mType{},
        mPart{},
        mTitle{},
        mObserver{}
    {}

    ~ArticleWriter() override = default;

    void Register(ArticleObserver *v) override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(ArticleObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] != nullptr)
                continue;

            this->mObserver[i] = v;
            break;
        }
    }

    void UnRegister(ArticleObserver *v) override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(ArticleObserver);

        this->Clean();
        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] != v)
                continue;

            this->mObserver[i] = nullptr;
            break;
        }
    }

    template<ArticleType _type = ArticleType::NONE>
    ArticleWriter &SetTitle(ArticleTitle &v)
    {
        this->mTitle = v;
        this->mType = _type;
        this->Notify();
        return (*this);
    }

    template<Jbool _isNew = false, ArticleType _type = ArticleType::NONE>
    ArticleWriter &SetPart(ArticlePart &v)
    {
        this->mPart = v;
        this->isNew = _isNew;
        this->mType = _type;
        this->Notify();
        return (*this);
    }

    ArticleWriter &Finish()
    {
        this->Done();
        return (*this);
    }

private:
    void Notify() override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(ArticleObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] == nullptr)
                continue;

            if (this->mType == ArticleType::TITLE)
                (*this->mObserver[i]).UpdateTitle(this->mTitle);
            else if (this->mType == ArticleType::PART)
                (*this->mObserver[i]).UpdatePart(this->isNew, this->mPart);
        }
    }

    void Done() override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(ArticleObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] == nullptr)
                continue;

            (*this->mObserver[i]).Finish();
        }
    }

    void Clean() override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(ArticleObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] == nullptr)
                continue;

            (*this->mObserver[i]).Clean();
        }
    }

private:
    Jbool isNew;
    ArticleType mType;
    ArticlePart mPart;
    ArticleTitle mTitle;
    ArticleObserver *mObserver[ArticleWriter_DEFAULT_SIZE];
};

class ArticleTitleStorage : public ArticleObserver
{
private:
    constexpr static Jchar ATRICLETITLESTORAGE_STORAGE_NAME[] = "storage_title_database";

public:
    ArticleTitleStorage() :
        mStorage{}
    {}

    ~ArticleTitleStorage() override = default;

private:
    void UpdateTitle(ArticleTitle &title) override
    {
        if (this->mStorage == nullptr)
            this->mStorage = fopen(ATRICLETITLESTORAGE_STORAGE_NAME, "wb");

        if (this->mStorage != nullptr)
            fwrite(title.GetArray(), title.GetLength(), 1, this->mStorage);
    }

    void Finish() override
    {
        if (this->mStorage != nullptr)
            fclose(this->mStorage);
    }

    void Clean() override
    {
        remove(ATRICLETITLESTORAGE_STORAGE_NAME);
    }

private:
    FILE *mStorage;
};

class ArticlePartStorage : public ArticleObserver
{
private:
    constexpr static Jchar ATRICLEPARTSTORAGE_STORAGE_NAME[] = "storage_part_database_%d";

public:
    ArticlePartStorage() :
        mStorage{},
        mTitleCount{},
        mTitleNumber{},
        mTitle{}
    {}

    ~ArticlePartStorage() override = default;

private:
    void UpdatePart(Jbool isNew, ArticlePart &part) override
    {
        if (isNew)
        {
            if (this->mStorage != nullptr)
                fclose(this->mStorage);

            this->mTitle.Clean();
            this->mTitle.Format(ATRICLEPARTSTORAGE_STORAGE_NAME, ++this->mTitleNumber);
            this->mStorage = fopen(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(this->mTitle.GetArray())), "wb");
        }

        if (this->mStorage != nullptr)
            fwrite(part.GetArray(), part.GetLength(), 1, this->mStorage);
    }

    void Finish() override
    {
        if (this->mStorage != nullptr)
        {
            fclose(this->mStorage);
            this->mStorage = nullptr;
        }

        this->mTitleNumber = 0;
    }

    void Clean() override
    {
        for (Jint i = 1; i < this->mTitleCount; ++i)
        {
            this->mTitle.Clean();
            this->mTitle.Format(ATRICLEPARTSTORAGE_STORAGE_NAME, i);
            if (remove(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(this->mTitle.GetArray()))) != 0)
                break;
        }
    }

private:
    FILE *mStorage;
    Jint mTitleCount;
    Jint mTitleNumber;
    ArticleTitle mTitle;
};

}

#endif //CXXEPUB_EPUBARTICLECOLLECTION_HPP
