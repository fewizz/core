#pragma once

#include "meta/is_enum_type.hpp"

template<typename T>
concept enum_type = is_enum_type<T>;