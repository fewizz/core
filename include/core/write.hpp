#pragma once

#include "meta/type/is_trivial.hpp"
#include "endianness.hpp"
#include "integer.hpp"
#include "span.hpp"

template<
	trivial Type, endianness Endianness = endianness::native, typename Iterator
>
void write(Type value, Iterator&& iterator) {
	for(uint8 byte : span{ (uint8*)&value, sizeof(Type) }) {
		*iterator++ = byte;
	}
}