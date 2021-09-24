#pragma once

#include "meta/is_type_enum.hpp"

template<typename T>
concept enum_type = is_type_enum<T>;