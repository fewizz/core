#pragma once

#include "../integer.hpp"

template<typename ArrayType, nuint Dimension>
inline constexpr nuint array_extent = __array_extent(ArrayType, Dimension);