#pragma once

#include "./range.hpp"

template<typename Type>
struct storage {
	alignas(Type) uint8 data[sizeof(Type)];

	auto iterator() const { return data; }
	auto iterator()       { return data; }

	auto sentinel() const { return data + sizeof(Type); }
	auto sentinel()       { return data + sizeof(Type); }
};