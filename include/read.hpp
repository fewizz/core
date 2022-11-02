#pragma once

#include "./__type/is_trivial.hpp"
#include "./__type/decay.hpp"
#include "./endianness.hpp"
#include "./integer.hpp"
#include "./__iterator/element_type.hpp"
#include "./__iterator/basic.hpp"

template<
	trivial Type, endianness Endianness = endianness::native,
	basic_iterator Iterator
>
requires (sizeof(decay<iterator_element_type<Iterator>>) == 1)
constexpr Type read(Iterator&& iterator) {
	alignas(Type) uint1a storage[sizeof(Type)];

	for(nuint byte_index = 0; byte_index < sizeof(Type); ++byte_index) {
		uint1a b = *iterator++;
		nuint index =
			endianness::native != Endianness ?
			sizeof(Type) - 1 - byte_index :
			byte_index;

		storage[index] = b;
	}

	Type value = __builtin_bit_cast(Type, storage);
	return value;
}