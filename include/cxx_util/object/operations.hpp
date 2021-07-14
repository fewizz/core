#pragma once

#include <bit>

#include "concepts.hpp"
#include "iterator/iterator.hpp"
#include "iterator/referencing_iterator.hpp"

namespace u { namespace obj {

template<u::trivial T, std::endian E = std::endian::native>
constexpr T read(u::atom_input_iterator auto it)  {
	alignas(T) std::byte bytes[sizeof(T)];

	auto out = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes + sizeof(T) - 1 };
		else
			return bytes;
	} ();

	std::copy_n(u::referencing_iterator{ it }, sizeof(T), out);
	++it;

	return *((T*) bytes);//std::bit_cast<T>(bytes); // TODO
}

template<u::object T, std::endian E = std::endian::native>
constexpr void write(const T& object, u::atom_output_iterator auto it) {
	auto bytes = (std::byte*) &object; // TODO std::bit_cast

	auto in = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes + sizeof(T) - 1 };
		else
			return bytes;
	} ();

	std::copy_n(in, sizeof(T), it);
}

}}