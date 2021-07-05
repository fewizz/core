#pragma once

#include "concepts.hpp"
#include <bit>
#include <iterator>
#include "iterator/iterator.hpp"
#include <array>

namespace u {

template<u::c::trivial T, std::endian E = std::endian::native>
constexpr T read_object(u::c::iterator_of_bytes auto it) {
	alignas(T) std::byte bytes[sizeof(T)];

	auto out = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes + sizeof(T) - 1 };
		else
			return bytes;
	} ();

	std::copy_n(it, sizeof(T), out);

	return std::bit_cast<T>(bytes);
}

template<u::c::object T, std::endian E = std::endian::native>
constexpr void write_object(const T& object, u::c::iterator_of_bytes auto it) {
	auto bytes = std::bit_cast<std::array<std::byte, 4>>(object);

	auto in = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes.end() - 1 };
		else
			return bytes.begin();
	} ();

	std::copy_n(in, sizeof(T), it);
}

}