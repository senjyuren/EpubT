#pragma once

#ifndef CXXEPUB_UTILSLOG_HPP
#define CXXEPUB_UTILSLOG_HPP

#include "../Kernel.hpp"

#include "UtilsJString.hpp"

namespace app::utils
{

enum class LogType
{
    ERRO,
    DBUG,
    INFO,
};

enum class LogLevelType
{
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
};

class Log
{
private:
    constexpr static Jsize LOG_SIZE = 4096;
    constexpr static Jsize LOG_TAG_SIZE = 32;
    constexpr static Jsize LOG_TIME_SIZE = 32;
    constexpr static Jsize LOG_SPLITS_SYMBOL_SIZE = 64;

    constexpr static Jchar LOG_DEFAULT_TAG[] = "Tools";
    constexpr static Jchar LOG_TIME_FORMAT[] = "%04d-%02d-%02d %02d:%02d:%02d";
    constexpr static Jchar LOG_LEVEL_INFO_FORMAT[] = "[%s][INFO][%s][%s]\r\n";
    constexpr static Jchar LOG_LEVEL_DBUG_FORMAT[] = "[%s][DBUG][%s][%s]\r\n";
    constexpr static Jchar LOG_LEVEL_ERRO_FORMAT[] = "[%s][ERRO][%s][%s]\r\n";

    constexpr static Jchar LOG_SPLITS_SYMBOL = '-';
    constexpr static Jchar LOG_SPLITS_LEVEL_1_FORMAT[] = "  %s\r\n";
    constexpr static Jchar LOG_SPLITS_LEVEL_2_FORMAT[] = "    %s\r\n";
    constexpr static Jchar LOG_SPLITS_LEVEL_3_FORMAT[] = "      %s\r\n";

    static Log &Instance()
    {
        static Log *obj = nullptr;
        if (obj == nullptr)
            obj = new Log();
        return (*obj);
    }

    Log &SetTag(JString<LOG_TAG_SIZE> v)
    {
        this->mTag = v;
        return (*this);
    }

    template<LogType _type, Jsize _size, typename... Args>
    Log &Output(JString<_size> &v, Args... args)
    {
        this->mLog.Clean();
        this->mLog.Format(v, args...);
        this->TimeFormat();
        if (_type == LogType::INFO)
            this->Info();
        else if (_type == LogType::DBUG)
            this->Debug();
        else if (_type == LogType::ERRO)
            this->Error();
        return (*this);
    }

public:
    virtual ~Log() = default;

    template<LogType _type, typename... Args>
    static void Print(JString<LOG_SIZE> v, Args...args)
    {
        Log::Instance().Output<_type>(v, args...);
    }

protected:
    JString<LOG_SIZE> mLog;
    JString<LOG_TAG_SIZE> mTag;
    JString<LOG_TIME_SIZE> mTime;

    Log() :
        mLog{},
        mTag{LOG_DEFAULT_TAG},
        mTime{}
    {}

    void TimeFormat()
    {
        auto &&nowTime = time(nullptr);
        auto &&localNowTime = localtime(&nowTime);
        if (localNowTime == nullptr)
            return;

        (*localNowTime).tm_year += 1900;
        ++(*localNowTime).tm_mon;
        this->mTime.Clean();
        this->mTime.Format<sizeof(LOG_TIME_FORMAT)>(
            LOG_TIME_FORMAT,
            (*localNowTime).tm_year,
            (*localNowTime).tm_mon,
            (*localNowTime).tm_mday,
            (*localNowTime).tm_hour,
            (*localNowTime).tm_min,
            (*localNowTime).tm_sec
        );
    }

    void Info()
    {
        printf(
            LOG_LEVEL_INFO_FORMAT,
            this->mTag.GetArray(),
            this->mTime.GetArray(),
            this->mLog.GetArray()
        );
    }

    void Debug()
    {
        printf(
            LOG_LEVEL_DBUG_FORMAT,
            this->mTag.GetArray(),
            this->mTime.GetArray(),
            this->mLog.GetArray()
        );
    }

    void Error()
    {
        printf(
            LOG_LEVEL_ERRO_FORMAT,
            this->mTag.GetArray(),
            this->mTime.GetArray(),
            this->mLog.GetArray()
        );
    }

    void Splits()
    {
        for (Jsize i = 0; i < LOG_SPLITS_SYMBOL_SIZE; ++i)
            printf("%c", LOG_SPLITS_SYMBOL);
        printf("\r\n");
    }

    template<LogLevelType _type = LogLevelType::LEVEL_1, typename...Args>
    void OutputFormat(const Jchar *v, Args...args)
    {
        this->mLog.Clean();
        this->mLog.Format(v, args...);
        if (_type == LogLevelType::LEVEL_1)
            printf(LOG_SPLITS_LEVEL_1_FORMAT, this->mLog.GetArray());
        else if (_type == LogLevelType::LEVEL_2)
            printf(LOG_SPLITS_LEVEL_2_FORMAT, this->mLog.GetArray());
        else if (_type == LogLevelType::LEVEL_3)
            printf(LOG_SPLITS_LEVEL_3_FORMAT, this->mLog.GetArray());
    }
};

}

#endif //CXXEPUB_UTILSLOG_HPP
