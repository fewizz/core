#pragma once

#include "../integer.hpp"

template<typename ArrayType, nuint Dimension>
static constexpr nuint array_extent = __array_extent(ArrayType, Dimension);