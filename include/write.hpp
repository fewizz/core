#pragma once

#include "./endianness.hpp"
#include "./type.hpp"
#include "./integer.hpp"
#include "./move.hpp"
#include "./output_stream.hpp"
#include "./bit_cast.hpp"
#include "./storage.hpp"
#include "__iterator/basic.hpp"
#include "__iterator/element_type.hpp"

template<
	trivial Type,
	endianness Endianness,
	basic_output_stream<uint1a> OS
>
constexpr void write(Type value, OS&& os) {
	for(nuint i = 0; i < sizeof(Type); ++i) {
		nuint i0 = Endianness != endianness::native ? sizeof(Type) - i - 1 : i;

		struct storage_t {
			alignas(Type) uint1a bytes[sizeof(Type)];
		};

		uint1a byte = bit_cast<storage_t>(value).bytes[i0];

		::write<uint1a, OS>(
			move(byte),
			forward<OS>(os)
		);
	}
}

// swapped arguments
template<
	trivial Type,
	endianness Endianness,
	basic_output_stream<uint1a> OS
>
constexpr void write(OS&& iterator, Type value) {
	write<Type, Endianness, OS>(value, forward<OS>(iterator));
}

template<
	endianness Endianness,
	trivial Type,
	basic_output_stream<uint1a> OS
>
constexpr void write(Type value, OS&& iterator) {
	write<Type, Endianness, OS>(value, forward<OS>(iterator));
}

template<
	endianness Endianness,
	trivial Type,
	basic_output_stream<uint1a> OS
>
requires (sizeof(decay<iterator_element_type<OS>>) == 1)
void write(OS&& iterator, Type value) {
	write<Type, Endianness, OS>(value, forward<OS>(iterator));
}