#pragma once

#include <cinttypes>

namespace util {

template<class T>
class flags {
    T res;
    using I = int64_t;

public:
    flags(auto... values) : res {
        (T)(I(0) | ... | (I)values)
    } {}

    flags& operator|(auto v) {
        res |= (T)v;
        return *this;
    }

    operator T() { return (T) res; }

    template<class T0>
    T0 to() { return (T0) res; }
};

}