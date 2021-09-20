#pragma once

#include <cstddef>

template<unsigned Size>
struct storage {
	char _[Size];
};

template<typename T>
using storage_for = storage<sizeof(T)>;