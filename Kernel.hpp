#pragma once

#ifndef KERNEL_HPP
#define KERNEL_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>

using Jsize = size_t;
using Jbool = bool;
using Jfloat = float;
using Jbyte = unsigned char;
using Juint = unsigned int;
using Julong = unsigned long;
using Jchar = char;
using Jint = int;
using Jlong = long;

void *operator new(Jsize v)
{
    auto &&obj = malloc(v);
    if (obj != nullptr)
        memset(obj, 0, v);
    return obj;
}

void operator delete(void *v) noexcept
{
    if (v != nullptr)
        free(v);
}

void operator delete(void *v, Jsize) noexcept
{
    if (v != nullptr)
        free(v);
}

void *operator new[](Jsize v)
{
    auto &&obj = malloc(v);
    if (obj != nullptr)
        memset(obj, 0, v);
    return obj;
}

void operator delete[](void *v) noexcept
{
    if (v != nullptr)
        free(v);
}

void operator delete[](void *v, Jsize) noexcept
{
    if (v != nullptr)
        free(v);
}

namespace kernel
{

template<typename T>
using ItType = T;

}

#endif //KERNEL_HPP
