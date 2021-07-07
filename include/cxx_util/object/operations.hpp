#pragma once

#include "concepts.hpp"
#include <bit>
#include <iterator>
#include "iterator/iterator.hpp"
#include <array>

namespace u {

template<u::c::trivial T, std::endian E = std::endian::native>
constexpr T object_from_bytes(u::c::iterator_of_bytes auto it) {
	alignas(T) std::byte bytes[sizeof(T)];

	auto out = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes + sizeof(T) - 1 };
		else
			return bytes;
	} ();

	std::copy_n(it, sizeof(T), out);

	return *((T*) bytes);//std::bit_cast<T>(bytes); // TODO
}

template<u::c::object T, std::endian E = std::endian::native>
constexpr void object_to_bytes(const T& object, u::c::iterator_of_bytes auto it) {
	auto bytes = (std::byte*) &object; // TODO std::bit_cast

	auto in = [&]() {
		if constexpr(E != std::endian::native)
			return std::reverse_iterator{ bytes + sizeof(T) - 1 };
		else
			return bytes;
	} ();

	std::copy_n(in, sizeof(T), it);
}

}