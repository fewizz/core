#pragma once

#include <cstddef>

template<std::size_t Size>
struct storage {
	char _[Size];
};

template<typename T>
using storage_for = storage<sizeof(T)>;