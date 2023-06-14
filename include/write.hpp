#pragma once

#include <endianness.hpp>
#include <type.hpp>
#include <integer.hpp>
#include <__iterator/basic.hpp>
#include <__iterator/element_type.hpp>

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

// swapped arguments
template<
	trivial Type, endianness Endianness = endianness::native,
	basic_iterator Iterator
>
requires (sizeof(decay<iterator_element_type<Iterator>>) == 1)
void write(Iterator&& iterator, Type value) {
	write<Type, Endianness, Iterator>(value, forward<Iterator>(iterator));
}

template<
	endianness Endianness,
	trivial Type,
	basic_iterator Iterator
>
requires (sizeof(decay<iterator_element_type<Iterator>>) == 1)
void write(Type value, Iterator&& iterator) {
	write<Type, Endianness, Iterator>(value, forward<Iterator>(iterator));
}

// swapped arguments
template<
	endianness Endianness,
	trivial Type,
	basic_iterator Iterator
>
requires (sizeof(decay<iterator_element_type<Iterator>>) == 1)
void write(Iterator&& iterator, Type value) {
	write<Type, Endianness, Iterator>(value, forward<Iterator>(iterator));
}