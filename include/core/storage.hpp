#pragma once

#include "integer.hpp"

template<uint Size>
struct storage {
	char _[Size];
};

template<typename T>
using storage_for = storage<sizeof(T)>;