#pragma once

#ifndef CXXEPUB_UTILSJSTRING_HPP
#define CXXEPUB_UTILSJSTRING_HPP

#include "../Kernel.hpp"

#include "UtilsJByteArrayBasic.hpp"

namespace app::utils
{

template<Jsize _BufSize>
class JString : public JByteArrayBasic<_BufSize>
{
protected:
    constexpr static Jsize JSTRING_DEFAULT_SIZE = 64;

public:
    using JByteArrayBasic<_BufSize>::JByteArrayBasic;

    JString() = default;

    ~JString() = default;

    JString(const Jchar *v)
    {
        if (v == nullptr)
            return;

        auto &&vLen = strlen(v);
        this->mLength = (vLen > _BufSize) ? (_BufSize) : (vLen);
        memcpy(this->mCache, v, this->mLength);
    }

    JString(const Jbyte *v)
    {
        if (v == nullptr)
            return;

        auto &&vLen = strlen(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(v)));
        this->mLength = (vLen > _BufSize) ? (_BufSize) : (vLen);
        memcpy(this->mCache, v, this->mLength);
    }

    template<Jsize _size, typename... Args>
    JString &Format(JString<_size> &v, Args...args)
    {
        auto &&tmpLen = _BufSize - this->mLength;
        this->mLength += snprintf(
            reinterpret_cast<Jchar *>(&this->mCache[this->mLength]),
            tmpLen,
            reinterpret_cast<Jchar *>(const_cast<Jbyte *>(v.GetArray())),
            args...
        );
        return (*this);
    }

    template<Jsize _size, typename... Args>
    JString &Format(JString<_size> &&v, Args...args)
    {
        auto &&tmpLen = _BufSize - this->mLength;
        this->mLength += snprintf(
            reinterpret_cast<Jchar *>(&this->mCache[this->mLength]),
            tmpLen,
            reinterpret_cast<Jchar *>(const_cast<Jbyte *>(v.GetArray())),
            args...
        );
        return (*this);
    }

    template<Jsize _size>
    Jbool Equals(JString<_size> &v)
    {
        Jint i = 0;

        if (v.GetLength() != this->mLength)
            return false;

        for (i = 0; i < static_cast<Jint>(this->mLength); ++i)
        {
            if (this->mCache[i] != v[i])
                break;
        }
        return (i == static_cast<Jint>(this->mLength));
    }

    Jbool Equals(JString<JSTRING_DEFAULT_SIZE> v)
    {
        return this->Equals<JSTRING_DEFAULT_SIZE>(v);
    }
};

}

#endif //CXXEPUB_UTILSJSTRING_HPP
