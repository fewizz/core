#pragma once

#include "integer.hpp"

template<nuint Size>
struct storage {
	char _[Size];
};

template<typename T>
using storage_for = storage<sizeof(T)>;