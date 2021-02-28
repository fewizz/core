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
	std::size_t N = sizeof(T)
>
constexpr std::optional<T>
read(
	u::iterator_of_bytes auto&& begin,
	u::iterator_of_bytes auto end
) {
	auto dist = std::distance(begin, end);

	if(dist < N) return { };

	if(E == std::endian::native) {
		return u::obj_representation<T>{ begin }.create();
	}

	std::advance(begin, sizeof(T));

	return u::obj_representation<T>{
		std::reverse_iterator(begin)
	}.create();
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
	std::endian E = std::endian::native,
	std::size_t N = sizeof(T)
>
constexpr void
write(
	T&& t,
	u::iterator_of_bytes auto&& begin,
	u::iterator_of_bytes auto end
) {
	auto dist = std::distance(begin, end);

	if(dist < N) throw std::out_of_range{""};

	if(E == std::endian::native) {
		std::ranges::copy(u::obj_representation<T>{ t }, begin);
		return;
	}

	std::advance(begin, sizeof(T));

	std::ranges::copy(
		u::obj_representation<T>{ t },
		std::reverse_iterator(begin)
	);
}

template<
	class T,
	std::endian E = std::endian::native,
	std::ranges::input_range R>
constexpr void
write(T&& t, R&& range) {
	write<T, E>(
		std::forward<T>(t),
		std::ranges::begin(range),
		std::ranges::end(range)
	);
}

#include <iterator>

template<std::input_iterator It>
struct bytes {
	u::byte_iterator<It> m_begin;
	u::byte_iterator<It> m_end;

	constexpr bytes(It begin, It end)
	:
		m_begin { begin },
		m_end { end }
		{}

	template<std::ranges::range R>
	constexpr bytes(const R& range)
	:
		m_begin { std::ranges::begin(range) },
		m_end { std::ranges::end(range) }
		{}

	template<class T, std::size_t N>
	constexpr bytes(T (&arr )[N])
	: bytes(arr, arr + N) {}

	constexpr auto begin() const { return m_begin; }
	constexpr auto end() const { return m_end; }
	constexpr auto size() const { return std::distance(m_begin, m_end); }

	template<class It0>
	constexpr std::strong_ordering
	operator <=> (const bytes<It0>& that) const {
		if(auto s = size() <=> that.size(); s != 0) return s;

		auto other_it = that.begin();

		for(auto v : *this) {
			auto other_v = *other_it;

			if(auto s = v <=> other_v; s != 0) return s;
			++other_it;
		}

		return std::strong_ordering::equal;
	}

	constexpr bool
	operator == (const bytes& that) const = default;
};

// deduction guides
template<std::ranges::range R>
bytes(const R& range) -> bytes<std::ranges::iterator_t<R>>;

template<class T, std::size_t N>
bytes(T (&)[N]) -> bytes<T*>;

}