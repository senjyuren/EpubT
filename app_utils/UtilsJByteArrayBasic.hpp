#pragma once

#ifndef CXXEPUB_UTILSJBYTEARRAYBASIC_HPP
#define CXXEPUB_UTILSJBYTEARRAYBASIC_HPP

#include "../Kernel.hpp"

namespace app::utils
{

template<Jsize _BufSize>
class JByteArrayBasic
{
public:
    JByteArrayBasic() :
        mLength{},
        mCache{}
    {}

    ~JByteArrayBasic() = default;

    explicit JByteArrayBasic(Jchar v) :
        mLength{},
        mCache{}
    {
        if (_BufSize < 1)
            return;

        this->mCache[0] = v;
        ++this->mLength;
    }

    explicit JByteArrayBasic(Jbyte v) :
        mLength{},
        mCache{}
    {
        if (_BufSize < 1)
            return;

        this->mCache[0] = v;
        ++this->mLength;
    }

    JByteArrayBasic(const Jchar *v, Jsize vLen) :
        mLength{},
        mCache{}
    {
        if (v == nullptr)
            return;

        this->mLength = vLen > _BufSize ? _BufSize : vLen;
        memcpy(this->mCache, v, this->mLength);
    }

    JByteArrayBasic(const Jbyte *v, Jsize vLen) :
        mLength{},
        mCache{}
    {
        if (v == nullptr)
            return;

        this->mLength = vLen > _BufSize ? _BufSize : vLen;
        memcpy(this->mCache, v, this->mLength);
    }

    template<Jsize _size>
    JByteArrayBasic(JByteArrayBasic<_size> &v):
        mLength{},
        mCache{}
    {
        this->mLength = v.GetLength() > _BufSize ? _BufSize : v.GetLength();
        memcpy(this->mCache, v.GetArray(), this->mLength);
    }

    template<Jsize _size>
    JByteArrayBasic(JByteArrayBasic<_size> &&v):
        mLength{},
        mCache{}
    {
        this->mLength = v.GetLength() > _BufSize ? _BufSize : v.GetLength();
        memcpy(this->mCache, v.GetArray(), this->mLength);
    }

    template<Jsize _size>
    JByteArrayBasic &operator=(JByteArrayBasic<_size> &v)
    {
        this->Clean();
        this->mLength = v.GetLength() > _BufSize ? _BufSize : v.GetLength();
        memcpy(this->mCache, v, this->mLength);
        return (*this);
    }

    template<Jsize _size>
    JByteArrayBasic &operator=(JByteArrayBasic<_size> &&v)
    {
        this->Clean();
        this->mLength = v.GetLength() > _BufSize ? _BufSize : v.GetLength();
        memcpy(this->mCache, v, this->mLength);
        return (*this);
    }

    Jbyte operator[](Jsize v)
    {
        if (v > this->mLength)
            return 0;
        return this->mCache[v];
    }

    JByteArrayBasic &Append(const Jbyte *v, Jsize vLen)
    {
        if (v == nullptr)
            return (*this);

        auto &&tmpLen = ((this->mLength + vLen) > _BufSize) ? (_BufSize - this->mLength) : (vLen);
        memcpy(&this->mCache[this->mLength], v, tmpLen);
        this->mLength += tmpLen;
        return (*this);
    }

    JByteArrayBasic &Append(const Jchar *v, Jsize vLen)
    {
        if (v == nullptr)
            return (*this);

        auto &&tmpLen = ((this->mLength + vLen) > _BufSize) ? (_BufSize - this->mLength) : (vLen);
        memcpy(&this->mCache[this->mLength], v, tmpLen);
        this->mLength += tmpLen;
        return (*this);
    }

    template<Jsize _size>
    JByteArrayBasic &Append(JByteArrayBasic<_size> &v)
    {
        auto &&tmpLen = ((this->mLength + v.GetLength()) > _BufSize)
                        ? (_BufSize - this->mLength)
                        : (v.GetLength());
        memcpy(&this->mCache[this->mLength], v, tmpLen);
        this->mLength += tmpLen;
        return (*this);
    }

    template<Jsize _size>
    JByteArrayBasic &Append(JByteArrayBasic<_size> &&v)
    {
        auto &&tmpLen = ((this->mLength + v.GetLength()) > _BufSize)
                        ? (_BufSize - this->mLength)
                        : (v.GetLength());
        memcpy(&this->mCache[this->mLength], v, tmpLen);
        this->mLength += tmpLen;
        return (*this);
    }

    JByteArrayBasic &Clean()
    {
        this->mLength = 0;
        memset(this->mCache, 0, sizeof(this->mCache));
        return (*this);
    }

    Jsize GetSize()
    {
        return _BufSize;
    }

    Jsize GetLength()
    {
        return this->mLength;
    }

    const Jbyte *GetArray()
    {
        return this->mCache;
    }

protected:
    Jsize mLength;
    Jbyte mCache[_BufSize + 1];
};

}

#endif //CXXEPUB_UTILSJBYTEARRAYBASIC_HPP
