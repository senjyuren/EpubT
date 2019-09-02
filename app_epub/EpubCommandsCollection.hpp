#pragma once

#ifndef CXXEPUB_EPUBCOMMANDSCOLLECTION_HPP
#define CXXEPUB_EPUBCOMMANDSCOLLECTION_HPP

#include "../Kernel.hpp"

namespace app::epub
{

class CommandsObserver;

class CommandsSubject
{
public:
    CommandsSubject() = default;

    virtual ~CommandsSubject() = default;

    virtual void Register(CommandsObserver *v) = 0;

    virtual void UnRegister(CommandsObserver *v) = 0;

    virtual void Notify() = 0;

    virtual void Done() = 0;
};

class CommandsObserver
{
public:
    CommandsObserver() = default;

    virtual ~CommandsObserver() = default;

    virtual void Update(CommandKey &command, CommandValue &value) = 0;

    virtual void Finish() = 0;
};

class CommandsHandle : public CommandsSubject
{
private:
    constexpr static Jsize COMMANDSHANDLE_DEFAULT_SIZE = 1024;

public:
    using CommandsSubject::CommandsSubject;

    CommandsHandle() :
        mPair{},
        mObserver{}
    {}

    ~CommandsHandle() override = default;

    void Register(CommandsObserver *v) override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(CommandsObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] != nullptr)
                continue;

            this->mObserver[i] = v;
            break;
        }
    }

    void UnRegister(CommandsObserver *v) override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(CommandsObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] != v)
                continue;

            delete (this->mObserver[i]);
            this->mObserver[i] = nullptr;
            break;
        }
    }

    CommandsHandle &SetKeyAndValue(CommandKey &key, CommandValue &value)
    {
        this->mPair.SetKey(key).SetValue(value);
        this->Notify();
        return (*this);
    }

    CommandsHandle &Finish()
    {
        this->Done();
        return (*this);
    }

private:
    void Done() override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(CommandsObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] == nullptr)
                continue;

            (*this->mObserver[i]).Finish();
        }
    }

    void Notify() override
    {
        auto &&size = sizeof(this->mObserver) / sizeof(CommandsObserver);

        for (Jsize i = 0; i < size; ++i)
        {
            if (this->mObserver[i] == nullptr)
                continue;

            (*this->mObserver[i]).Update(this->mPair.GetKey(), this->mPair.GetValue());
        }
    }

private:
    CommandsPair mPair;
    CommandsObserver *mObserver[COMMANDSHANDLE_DEFAULT_SIZE];
};

class CommandsHelp : public CommandsObserver
{
public:
    explicit CommandsHelp() = default;

    ~CommandsHelp() override = default;

private:
    void Update(CommandKey &key, CommandValue &) override
    {
        if (Define::ConvKey(key) == DefineKeyType::HELP)
            this->mHelp = key;
    }

    void Finish() override
    {
        if (!this->mHelp.IsEmpty())
            this->Help();
    }

    void Help()
    {
        DefineHelpGenerator::Output<DefineKeyType::HELP>();
    }

private:
    CommandKey mHelp;
};

class CommandsXhtml : public CommandsObserver
{
public:
    CommandsXhtml() :
        mHelp{},
        mHead{},
        mMode{}
    {}

    ~CommandsXhtml() override = default;

private:
    void Update(CommandKey &key, CommandValue &value) override
    {
        if (Define::ConvKey(key) == DefineKeyType::HELP)
            this->mHelp = key;
        else if (Define::ConvKey(key) == DefineKeyType::MODE)
            this->mMode.SetKey(key).SetValue(value);
        else if (Define::ConvKey(key) == DefineKeyType::HEAD)
            this->mHead = key;
    }

    void Finish() override
    {
        if ((!this->mHelp.IsEmpty()) &&
            ((this->mMode.GetKey().IsEmpty())
             || (Define::ConvValue(this->mMode.GetValue()) == DefineValueType::XHTML)))
            this->Help();
        else
            this->Xhmlt();
    }

    void Help()
    {
        DefineHelpGenerator::Output<DefineKeyType::NONE, DefineValueType::XHTML>();
    }

    void Xhmlt()
    {
    }

private:
    CommandKey mHelp;
    CommandKey mHead;

    CommandsPair mMode;
};

class CommandsSplit : public CommandsObserver
{
public:
    CommandsSplit() :
        mHelp{},
        mMode{},
        mInputFile{},
        mOutputFolder{}
    {}

    ~CommandsSplit() override = default;

private:
    void Update(CommandKey &key, CommandValue &value) override
    {
        if (Define::ConvKey(key) == DefineKeyType::HELP)
            this->mHelp = key;
        else if (Define::ConvKey(key) == DefineKeyType::MODE)
            this->mMode.SetKey(key).SetValue(value);
        else if (Define::ConvKey(key) == DefineKeyType::INPUT_FILE)
            this->mInputFile.SetKey(key).SetValue(value);
        else if (Define::ConvKey(key) == DefineKeyType::OUTPUT_FOLDER)
            this->mOutputFolder.SetKey(key).SetValue(value);
    }

    void Finish() override
    {
        if ((!this->mHelp.IsEmpty()) &&
            ((this->mMode.GetKey().IsEmpty())
             || (Define::ConvValue(this->mMode.GetValue()) == DefineValueType::SPLIT)))
            this->Help();
        else
            this->Split();
    }

    void Help()
    {
        DefineHelpGenerator::Output<DefineKeyType::NONE, DefineValueType::SPLIT>();
    }

    void Split()
    {
    }

private:
    CommandKey mHelp;

    CommandsPair mMode;
    CommandsPair mInputFile;
    CommandsPair mOutputFolder;
};

}

#endif //CXXEPUB_EPUBCOMMANDSCOLLECTION_HPP
