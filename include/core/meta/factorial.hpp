#pragma once

#include "../integer.hpp"

template<nuint Initial>
constexpr inline nuint factorial = Initial * factorial<Initial - 1>;

template<>
constexpr inline nuint factorial<1> = 1;