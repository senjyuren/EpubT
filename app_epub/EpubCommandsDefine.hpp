#pragma once

#ifndef CXXEPUB_EPUBCOMMANDSDEFINE_HPP
#define CXXEPUB_EPUBCOMMANDSDEFINE_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsLog.hpp"

#include "EpubBase.hpp"
#include "EpubCommandsBasic.hpp"

namespace app::epub
{

enum class DefineKeyType
{
    NONE,
    HELP,
    MODE,

    HEAD,
    SYMBOL,
    INPUT_FILE,
    OUTPUT_FOLDER,
};

enum class DefineValueType
{
    NONE,
    XHTML,
    SPLIT,
};

typedef struct
{
    typedef union
    {
        DefineKeyType mKey;
        DefineValueType mValue;
    } Attr;

    Attr mAttr;
    const Jchar *mStr;
} DefineBasic;

class Define
{
private:
    constexpr static DefineBasic DEFINE_KEYS[] =
        {{{.mKey = DefineKeyType::HELP},          "help"},
         {{.mKey = DefineKeyType::MODE},          "mode"},
         {{.mKey = DefineKeyType::HEAD},          "head"},
         {{.mKey = DefineKeyType::SYMBOL},        "symbol"},
         {{.mKey = DefineKeyType::INPUT_FILE},    "input_file"},
         {{.mKey = DefineKeyType::OUTPUT_FOLDER}, "output_folder"},};

    constexpr static DefineBasic DEFINE_VALUES[] =
        {{{.mValue = DefineValueType::XHTML}, "xhtml"},
         {{.mValue = DefineValueType::SPLIT}, "split"},};

    Define() = default;

    ~Define() = default;

public:
    static DefineKeyType ConvKey(CommandKey &key)
    {
        for (const auto &v : DEFINE_KEYS)
        {
            if (key.Equals(v.mStr))
                return v.mAttr.mKey;
        }
        return DefineKeyType::NONE;
    }

    static DefineValueType ConvValue(CommandValue &key)
    {
        for (const auto &v : DEFINE_VALUES)
        {
            if (key.Equals(v.mStr))
                return v.mAttr.mValue;
        }
        return DefineValueType::NONE;
    }
};

class DefineHelpGenerator : public app::utils::Log
{
private:
    template<Jsize _size>
    using JString = app::utils::JString<_size>;

    using LogLevelType = app::utils::LogLevelType;

private:
    static DefineHelpGenerator &Instance()
    {
        static DefineHelpGenerator *obj = nullptr;
        if (obj == nullptr)
            obj = new DefineHelpGenerator();
        return (*obj);
    }

    using Log::Log;

    DefineHelpGenerator() = default;

    ~DefineHelpGenerator() override = default;

public:
    template<DefineKeyType _kt = DefineKeyType::HELP, DefineValueType _vt = DefineValueType::NONE>
    static void Output()
    {
        if (_kt == DefineKeyType::HELP)
            DefineHelpGenerator::Instance().Help();
        else if (_vt == DefineValueType::XHTML)
            DefineHelpGenerator::Instance().Xhtml();
        else if (_vt == DefineValueType::SPLIT)
            DefineHelpGenerator::Instance().Split();
    }

private:
    void Help()
    {
        this->Splits();
        this->OutputFormat("Version: %s\r\n", Property::Get<PropertyType::VERSION>().GetArray());
    }

    void Xhtml()
    {
        this->Splits();
        this->OutputFormat("Xhtml: -mode=xhtml");
        this->OutputFormat<LogLevelType::LEVEL_2>("-head");
        this->OutputFormat<LogLevelType::LEVEL_3>("[If you has set it, so to generate file has the xhtml head.]");
    }

    void Split()
    {
        this->Splits();
        this->OutputFormat("Split: -mode=split");
        this->OutputFormat<LogLevelType::LEVEL_2>("-symbol=##split##");
        this->OutputFormat<LogLevelType::LEVEL_3>("[If you has set it, self have to use this symbol to split file.]");
        this->OutputFormat<LogLevelType::LEVEL_2>("-input_file=D:\\");
        this->OutputFormat<LogLevelType::LEVEL_3>("[This is you to split file of path.]");
        this->OutputFormat<LogLevelType::LEVEL_2>("-output_folder=D:\\");
        this->OutputFormat<LogLevelType::LEVEL_3>("[This is split done file output dirctory.]");
    }
};

}

#endif //CXXEPUB_EPUBCOMMANDSDEFINE_HPP
