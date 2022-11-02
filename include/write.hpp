#pragma once

#include "./__iterator/basic.hpp"
#include "./__type/is_trivial.hpp"
#include "./__type/decay.hpp"
#include "./endianness.hpp"
#include "./integer.hpp"
#include "./__iterator/element_type.hpp"

template<
	trivial Type, endianness Endianness = endianness::native,
	basic_iterator Iterator
>
requires (sizeof(decay<iterator_element_type<Iterator>>) == 1)
void write(Type value, Iterator&& iterator) {
	for(nuint i = 0; i < sizeof(Type); ++i) {
		nuint i0 = Endianness != endianness::native ? sizeof(Type) - i - 1 : i;
		*iterator++ = ((uint1a*) &value)[i0];
	}
}