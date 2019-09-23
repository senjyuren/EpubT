#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

using Jchar  = char;
using Jint   = int;
using Jlong  = long;
using Jbyte  = unsigned char;
using Juint  = unsigned int;
using Julong = unsigned long;
using Jsize  = size_t;
using Jbool  = bool;

void *operator new(Jsize v)
{
    auto &&obj = malloc(v);
    if(obj != nullptr)
        memset(obj,0,v);
    return obj;
}

void operator delete(void *v) noexcept
{
    if(v != nullptr)
        free(v);
}

void *operator new[](Jsize v)
{
    auto &&obj = malloc(v);
    if(obj != nullptr)
        memset(obj,0,v);
    return obj;
}

void operator delete[](void *v) noexcept
{
    if(v != nullptr)
        free(v);
}


namespace kernel
{

template<typename T>
using ItType = T;

}

namespace lang
{

template<typename _Type,Jint _BufSize>
class StackType
{
public:
    StackType()
        :mLength{}
        ,mArray{}
    {}

    virtual ~StackType() = default;

protected:
    Jint  mLength;
    _Type mArray[_BufSize + 1];
};

template<Jint _BufSize>
class StringBase : public StackType<Jbyte,_BufSize>
{
public:
    using StackType<Jbyte,_BufSize>::StackType;

    StringBase() = default;

    ~StringBase() override = default;

    StringBase(const Jchar *v)
    {
        if(v == nullptr)
            return;

        auto &&vLen = strlen(v);
        this->mLength = _BufSize > vLen ? vLen : _BufSize;
        memcpy(this->mArray,v,this->mLength);
    }
    
    StringBase(const Jbyte *v)
        :StringBase{reinterpret_cast<Jchar *>(const_cast<Jbyte *>(v))}
    {}

    template<Jint _size>
    StringBase(StringBase<_size> &v)
    {
        this->mLength = _BufSize > v.GetLength() ? v.GetLength() : _BufSize;
        memcpy(this->mArray,v,this->mLength);
    }

    template<Jint _size>
    StringBase(StringBase<_size> &&v)
        :StringBase{reinterpret_cast<StringBase<_size> &>(v)}
    {}

    StringBase &Append(const Jchar *v)
    {
        if(v == nullptr)
            return (*this);

        auto &&vLen = strlen(v);
        auto &&tmpLen = (this->mLength + vLen) > _BufSize
            ? _BufSize - this->mLength
            : vLen;

        memcpy(&this->mArray[this->mLength],v,tmpLen);
        this->mLength += vLen;
        return (*this);
    }

    StringBase &Append(const Jbyte *v)
    {
        return this->Append(reinterpret_cast<Jchar *>(const_cast<Jbyte *>(v)));
    }

    template<Jint _size>
    StringBase &Append(StringBase<_size> &v)
    {
        auto &&tmpLen = (this->mLength + v.GetLength()) > _BufSize
            ? _BufSize - this->mLength;
            : v.GetLength();
        
        memcpy(&this->mArray[this->mLength],v.GetArray(),tmpLen);
        this->mLength += tmpLen;
        return (*this);
    }

    template<Jint _size>
    StringBase &Append(StringBase<_size> &&v)
    {
        return this->Append(reinterpret_cast<StringBase<_size> &>(v));
    }

    StringBase &Clean()
    {
        this->mLength = 0;
        memset(this->mArray,0,sizeof(this->mArray));
        return (*this);
    }

    Jint GetSize()
    {
        return _BufSize;
    }

    Jint GetLength()
    {
        return this->mLength;
    }

    const Jchar *GetString()
    {
        return reinterpret_cast<Jchar *>(this->mArray);
    }

    const Jbyte *GetArray()
    {
        return this->mArray;
    }
};

template<Jint _BufSize>
class String : public StringBase<_BufSize>
{
public:
    using StringBase<_BufSize>::StringBase;

    String() = default;

    ~String() override = default;

    template<Jint _size>
    String &operator=(String<_size> &v)
    {
        this->Clean();
        this->mLength = _BufSize > v.GetLength() ? v.GetLength() : _BufSize;
        memcpy(this->mArray,v.GetArray(),this->mLength);
        return (*this);
    }

    template<Jint _size>
    String &operator=(String<_size> &&v)
    {
        return this->operator=(reinterpret_cast<String<_size> &>(v));
    }
};

}

namespace feature
{

class FilePath : public lang::String<1024>
{
public:
    using String::String;

    FilePath() = default;

    ~FilePath() override = default;
};

enum FileMode
{
    FILEMODE_READ  = 0x01,
    FILEMODE_WRITE = 0x02,
};

class FileModeCmd : public lang::String<16>
{
public:
    using String::String;

    FileModeCmd() = default;

    ~FileModeCmd() override = default;
};

class FileRow : public lang::String<4096>
{
public:
    using String::String;

    FileRow() = default;

    ~FileRow() override = default;
};

template<Jint _BufSize>
class FileRows : public lang::StackType<FileRow,_BufSize>
{
public:
    using lang::StackType<FileRow,_BufSize>::StackType;

    FileRows() = default;

    ~FileRows() override = default;

    FileRow &operator[](Jint i)
    {
        if(i > this->mLength)
            return this->mArray[_BufSize];
        return this->mArray[i];
    }
};

template<Jsize _BufSize>
class FileBase
{
public:
    FileBase()
        :isOpen{}
        ,mRow{}
        ,mRows{}
        ,mPath{}
        ,mMode{}
        ,mModeCmd{}
        ,mHandle{}
    {}

    virtual ~FileBase() = default;

protected:
    Jbool              isOpen;
    FileRow            mRow;
    FileRows<_BufSize> mRows;

    FilePath           mPath;
    FileMode           mMode;
    FileModeCmd        mModeCmd;
    FILE              *mHandle;
};

template<Jint _BufSize>
class File : public FileBase<_BufSize>
{
public:
    using FileBase<_BufSize>::FileBase;

    File() = default;

    ~File() override
    {
        if(this->mHandle != nullptr)
            fclose(this->mHandle);
    }
    
    File(FilePath &v)
    {
        this->mPath = v;
    }

    File(FilePath &&v)
        :File{reinterpret_cast<FilePath &>(v)}
    {}

    void Close()
    {
        if(this->mHandle != nullptr)
            fclose(this->mHandle);
        this->mHandle = nullptr;
    }

    Jbool Open(FilePath &v)
    {
        this->mPath = v;
        return this->Open();
    }

    Jbool Open(FilePath &&v)
    {
        return this->Open(reinterpret_cast<FilePath &>(v));
    }

    Jbool Open()
    {
        this->isOpen = (this->mHandle = fopen(this->mPath.GetString(),this->mModeCmd.GetString())) == nullptr;
        return this->isOpen;
    }
    
    File &SetMode(enum FileMode mode)
    {
        if((mode & FILEMODE_READ) == FILEMODE_READ)
            this->mModeCmd.Append("r");
        if((mode & FILEMODE_WRITE) == FILEMODE_WRITE)
            this->mModeCmd.Append("w");
        return (*this);
    }

    FileRow &ReadRow()
    {
        return this->nRow;
    }

    File &WriteRow()
    {
        return (*this);
    }

    FileRows<_BufSize> &ReadRows()
    {
        return this->mRows;
    }
};

}
