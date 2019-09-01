#pragma once

#ifndef CXXEPUB_UTILSJBYTEARRAY_HPP
#define CXXEPUB_UTILSJBYTEARRAY_HPP

#include "../Kernel.hpp"

namespace app::utils
{

template<Jsize _BufSize>
class JByteArray : public JByteArrayBasic<_BufSize>
{
    JByteArray() = default;

    ~JByteArray() = default;
};

}

#endif //CXXEPUB_UTILSJBYTEARRAY_HPP
