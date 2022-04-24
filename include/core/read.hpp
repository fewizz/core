#pragma once

#include "meta/type/is_trivial.hpp"
#include "endianness.hpp"
#include "integer.hpp"

template<trivial Type, typename Iterator>
Type read(Iterator&& iterator) {
	alignas(Type) uint8 storage[sizeof(Type)];

	for(nuint byte_index = 0; byte_index < sizeof(Type); ++byte_index) {
		uint8 b = *iterator++;
		nuint index =
			endianness::native == endianness::little ?
			sizeof(Type) - 1 - byte_index :
			byte_index;

		storage[index] = b;
	}

	Type value = *((Type*) storage);
	return value;
}