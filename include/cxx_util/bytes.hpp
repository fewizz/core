#pragma once

#include <bit>
#include <compare>
#include <iterator>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <tl/expected.hpp>
#include <type_traits>
#include "bit.hpp"
#include "byte.hpp"
#include "encoding/request_error.hpp"
#include "iterator.hpp"
#include "byte_iterator.hpp"
#include <climits>
#include "obj_representation.hpp"

static_assert( CHAR_BIT == 8 );

namespace u {

template<
	class T,
	std::endian E = std::endian::native,
	std::size_t N = sizeof(T),
	class ItBegin
> requires ( u::iterator_of_bytes<std::remove_reference_t<ItBegin>> )
constexpr std::optional<T>
read(
	ItBegin&& begin,
	u::iterator_of_bytes auto end
) {
	auto dist = std::distance(begin, end);

	if(dist < N) return { };

	auto begin_copy = begin;
	std::advance(begin, N);

	if(E == std::endian::native) {
		return u::read_object<T>(begin_copy);
	}

	return u::read_object<T>(std::reverse_iterator(begin));
}

template<
	class T,
	std::endian E = std::endian::native,
	std::ranges::input_range R>
constexpr std::optional<T>
read(R&& range) {
	return read<T, E>(
		std::ranges::begin(range),
		std::ranges::end(range)
	);
}

template<
	class T,
	std::size_t N = sizeof(T),
	std::endian E = std::endian::native,
	class BeginIt
> requires( u::iterator_of_bytes<std::remove_reference_t<BeginIt>> )
constexpr void
write(
	T t,
	BeginIt&& begin,
	u::iterator_of_bytes auto end
) {
	auto dist = std::distance(begin, end);

	if(dist < N) throw std::out_of_range{""};

	auto begin_copy = begin;
	std::advance(begin, N);

	if(E == std::endian::native) {
		std::ranges::copy(u::obj_representation_copy{ t }, begin_copy);
		return;
	}

	std::ranges::copy(
		u::obj_representation_copy{ t },
		std::reverse_iterator(begin)
	);
}

template<
	class T,
	std::endian E = std::endian::native,
	class BeginIt
> requires( u::iterator_of_bytes<std::remove_reference_t<BeginIt>> )
constexpr void
write(
	T t,
	BeginIt&& begin,
	u::iterator_of_bytes auto end
) {
	write<T, sizeof(T), E>(t, std::forward<BeginIt>(begin), end);
}

template<
	std::endian E = std::endian::native,
	class BeginIt
> requires( u::iterator_of_bytes<std::remove_reference_t<BeginIt>> )
constexpr void
write(
	auto t,
	BeginIt&& begin,
	u::iterator_of_bytes auto end
) {
	write<decltype(t), sizeof(t), E>(t, std::forward<BeginIt>(begin), end);
}

template<
	class T,
	std::size_t N,
	std::endian E = std::endian::native,
	std::ranges::input_range R>
constexpr void
write(T t, R&& range) {
	write<T, N, E>(
		std::forward<T>(t),
		std::ranges::begin(range),
		std::ranges::end(range)
	);
}

template<
	class T,
	std::endian E = std::endian::native,
	std::ranges::input_range R>
constexpr void
write(T t, R&& range) {
	write<T, sizeof(T), E>(
		std::forward<T>(t),
		std::ranges::begin(range),
		std::ranges::end(range)
	);
}

}