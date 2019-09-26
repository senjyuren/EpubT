#pragma once

#ifndef CXXEPUB_EPUBBASE_HPP
#define CXXEPUB_EPUBBASE_HPP

#include "../Kernel.hpp"

#include "../app_utils/UtilsJString.hpp"

namespace app::epub
{

enum class PropertyType
{
    VERSION,
};

class Property
{
private:
    template<Jsize _size>
    using JString = app::utils::JString<_size>;

    constexpr static Jsize PROPERTY_SIZE = 64;

    constexpr static Jchar PROPERTY_VERSION[] = "1.0.0";

private:
    static Property &Instance()
    {
        static Property *obj = nullptr;
        if (obj == nullptr)
            obj = new Property();
        return (*obj);
    }

    Property() = default;

    virtual ~Property() = default;

    template<PropertyType _type>
    JString<PROPERTY_SIZE> GetValue()
    {
        if (_type == PropertyType::VERSION)
            return PROPERTY_VERSION;
    }

public:
    template<PropertyType _type>
    static JString<PROPERTY_SIZE> Get()
    {
        return Property::Instance().GetValue<_type>();
    }
};

}

#endif //CXXEPUB_EPUBBASE_HPP
