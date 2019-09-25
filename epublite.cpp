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

template<typename _Type>
class HeapTypeBase
{
public:
    HeapTypeBase()
        :mLength{}
        ,mArray{}
    {}

    virtual ~HeapTypeBase()
    {
        if(this->mArray != nullptr)
            delete[](this->mArray);
        this->mLength = 0;
        this->mArray = nullptr;
    }

protected:
    Jint   mLength;
    _Type *mArray;
};

template<typename _Type>
class HeapType : public HeapTypeBase<_Type>
{
public:
    using HeapTypeBase<_Type>::HeapTypeBase;

    HeapType() = default;

    ~HeapType() override = default;

    explicit HeapType(Jsize v)
    {
        this->mLength = v;
        this->mArray = new _Type[this->mLength + 1];
    }

    void Make(Jsize v)
    {
        if(this->mArray != nullptr)
            delete[](this->mArray);

        this->mLength = v;
        this->mArray = new _Type[v];
    }

    Jint GetSize()
    {
        return this->mLength;
    }
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

    StringBase &Append(Jchar v)
    {
        if(this->mLength > _BufSize)
            return (*this);

        this->mArray[this->mLength] = v;
        ++this->mLength;
        return (*this);
    }

    StringBase &Append(Jbyte v)
    {
        return this->Append(static_cast<Jchar>(v));
    }

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

enum class FileMode
{
    READ,
    WRITE,
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

class FileRows : public lang::HeapType<FileRow>
{
public:
    using lang::HeapType<FileRow>::HeapType;

    FileRows() = default;

    ~FileRows() override = default;

    FileRow &operator[](Jint i)
    {
        if((this->mLength < 1) || (this->mArray == nullptr))
            return this->mArray[this->mLength];
        if(i > this->mLength)
            return this->mArray[this->mLength];
        return this->mArray[i];
    }
};

class FileBase
{
public:
    FileBase()
        :mChar{}
        ,mRow{}
        ,mRows{}
        ,mPath{}
        ,mMode{}
        ,mModeCmd{}
        ,mHandle{}
    {}

    virtual ~FileBase() = default;

protected:
    Jbyte        mChar;
    FileRow      mRow;
    FileRows     mRows;

    FilePath     mPath;
    FileMode     mMode;
    FileModeCmd  mModeCmd;
    FILE        *mHandle;
};

class File : public FileBase
{
public:
    using FileBase::FileBase;

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
        return ((this->mHandle = fopen(this->mPath.GetString(),this->mModeCmd.GetString())) != nullptr);
    }
    
    File &SetMode(FileMode mode)
    {
        if(mode == FileMode::READ)
            this->mModeCmd.Append("rb");
        else if(mode == FileMode::WRITE)
            this->mModeCmd.Append("wb+");
        return (*this);
    }

    FileRow &ReadRow()
    {
        Jint ret = 0;
        Jbool mark = false;

        this->mRow.Clean();
        if(this->mHandle == nullptr)
            return this->mRow;
        
        do
        {
            ret = fread(&this->mChar,1,1,this->mHandle);
            if((this->mChar != 0x0A) && (this->mChar != 0x0D))
                this->mRow.Append(this->mChar);
            else
                mark = this->mChar == 0x0A;

            if(mark)
            {
                if(ret = fread(&this->mChar,1,1,this->mHandle);ret < 1)
                    break;
                if(this->mChar != 0x0D)
                    fseek(this->mHandle,-1,SEEK_CUR);
            }
        }while((ret > 0) && (!mark));
        return this->mRow;
    }

    File &WriteRow(FileRow &v)
    {
        return this->WriteRow(reinterpret_cast<FileRow &&>(v));
    }

    File &WriteRow(FileRow &&)
    {
        return (*this);
    }

    FileRows &ReadRows()
    {
        return this->mRows;
    }
};

}

Jint main()
{
    using feature::File;
    using feature::FilePath;
    using feature::FileMode;

    FilePath localFile = "F:\\2620.txt";
    File localFileHandle{localFile};

    do
    {
        localFileHandle.SetMode(FileMode::READ);
        if(!localFileHandle.Open())
            break;
        
        auto &&oneRow = localFileHandle.ReadRow();
        printf("%s\r\n",oneRow.GetString());
        
        auto &&twoRow = localFileHandle.ReadRow();
        printf("%s\r\n",twoRow.GetString());
        
        auto &&threeRow = localFileHandle.ReadRow();
        printf("%s\r\n",threeRow.GetString());
    }while(false);
    return 0;
}
