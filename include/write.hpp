#pragma once

#include "./type/is_trivial.hpp"
#include "./endianness.hpp"
#include "./integer.hpp"

template<
	trivial Type, endianness Endianness = endianness::native, typename Iterator
>
void write(Type value, Iterator&& iterator) {
	for(nuint i = 0; i < sizeof(Type); ++i) {
		nuint i0 = Endianness != endianness::native ? sizeof(Type) - i - 1 : i;
		*iterator++ = ((uint8*) &value)[i0];
	}
}