#pragma once

#include <bit>
#include <bits/c++config.h>
#include <bits/iterator_concepts.h>
#include <bits/stdint-uintn.h>
#include <codecvt>
#include <compare>
#include <cstddef>
#include <iterator>
#include <cstring>
#include <memory>
#include <tl/expected.hpp>
#include <type_traits>
#include "bit.hpp"
#include "encoding/request_error.hpp"
#include <climits>

static_assert( CHAR_BIT == 8 );

namespace util {

template<class It, std::endian Endian>
class byte_ref {
	//uint8_t m_value;
	using prev_value_type = std::iter_value_t<It>;
	constexpr static unsigned prev_value_type_size = sizeof(prev_value_type);
	uint8_t m_byte_index;
	It m_base;
public:

	constexpr byte_ref() = default;
	constexpr byte_ref(It it, uint8_t in)
		: m_byte_index{ in }, m_base { it } {}

	constexpr byte_ref& operator == (std::byte v) {
		util::
	}

	constexpr operator std::byte () const {
		return util::read_byte(
			*m_base,
			Endian == std::endian::little ?
				m_byte_index :
				prev_value_type_size - m_byte_index - 1
		);
	}
};

template<class It, std::endian Endian = std::endian::little>
struct bytes_visitor_iterator {
private:
	It m_base;
	unsigned byte_index = 0;

public:
	using prev_value_type = std::iter_value_t<It>;
	constexpr static unsigned prev_value_type_size = sizeof(prev_value_type);

	using value_type = byte_ref<It, Endian>;
	using difference_type = std::ptrdiff_t;
	//using iterator_category = std::input_iterator_tag;

	using base_iterator_category = typename std::iterator_traits<It>::iterator_category;

	static constexpr bool base_is_random_access_iterator =
		std::derived_from<base_iterator_category, std::random_access_iterator_tag>;

	constexpr bytes_visitor_iterator() {}
	constexpr bytes_visitor_iterator(It iter) : m_base { iter } {}

protected:
	constexpr bytes_visitor_iterator(It iter, unsigned byte_index)
		: m_base { iter }, byte_index { byte_index } {}

public:
	constexpr auto base() const { return m_base; }

	constexpr value_type
	operator * () const {
		return { *this, byte_index };
	}

	constexpr bytes_visitor_iterator&
	operator ++ () {
		++byte_index;

		if(byte_index >= prev_value_type_size) {
			++m_base;
			byte_index = 0;
		}

		return *this;
	}

	constexpr bytes_visitor_iterator&
	operator -- () {
		if(byte_index == 0) {
			--m_base;
			byte_index = prev_value_type_size;
		}

		--byte_index;

		return *this;
	}

	constexpr bytes_visitor_iterator
	operator ++ (int) {
		bytes_visitor_iterator prev = *this;
		++(*this);
		return prev;
	}

	constexpr bytes_visitor_iterator
	operator -- (int) {
		bytes_visitor_iterator prev = *this;
		--(*this);
		return prev;
	}

	constexpr bytes_visitor_iterator
	operator + (difference_type n) const
	requires( base_is_random_access_iterator ) {
		bytes_visitor_iterator copy = *this;
		return copy += n;
	}

	constexpr bytes_visitor_iterator
	operator - (difference_type n) const
	requires( base_is_random_access_iterator ) {
		bytes_visitor_iterator copy = *this;
		return copy -= n;
	}

	constexpr difference_type
	operator - (const bytes_visitor_iterator& that) const 
	requires( base_is_random_access_iterator ) {
		return (m_base - that.iter) * prev_value_type_size + (byte_index - that.byte_index);
	}

	constexpr bytes_visitor_iterator&
	operator += (difference_type n)
	requires( base_is_random_access_iterator ) {
		auto div = [](auto first, auto second) {
			auto d = first / second;
			auto r = first % second;

			if(first < 0) {
				if(r != 0) --d;
				r = second + r;
			}

			return std::pair{ d, r };
		};

		auto n0 = n + byte_index;
		auto [iter_adv, rem] = div(n0, prev_value_type_size);

		std::advance(m_base, iter_adv);
		byte_index = rem;

		return *this;
	}

	constexpr bytes_visitor_iterator&
	operator -= (difference_type n)
	requires( base_is_random_access_iterator ) {
		return *this += -n;
	}

	constexpr std::byte operator [] (std::ptrdiff_t n) const {
		return *this + n;
	}

	auto operator <=> (const bytes_visitor_iterator& that) const = default;
};

template<class BaseIt>
constexpr auto operator + (std::ptrdiff_t n, const bytes_visitor_iterator<BaseIt>& that) {
	return that + n;
}

template<class R>
concept byte_range =
	std::ranges::range<R> &&
	std::is_same_v<std::ranges::range_value_t<R>, std::byte>;

template<class It>
concept byte_iterator =
	std::input_iterator<It> && std::same_as<std::iter_value_t<It>, std::byte>;

template<std::endian E, std::integral T, std::size_t N = sizeof(T)>
constexpr std::optional<T>
next(byte_iterator auto& begin, byte_iterator auto end) {
	std::size_t dist = std::distance(begin, end);

	if(dist < N) return { };

	T res = T(*begin);
	++begin;
	
	for(std::size_t i = 1; i < N; i++) {
		if constexpr (E == std::endian::little) {
			res |= T(*begin) << ( 8 * i );
		}
		else {
			res <<= 8;
			res |= T(*begin);
		}
		++begin;
	}

	return { res };
}

template<std::integral T> requires(sizeof(T) == 1)
constexpr std::optional<T>
next(byte_iterator auto& begin, byte_iterator auto end) {
	return next<std::endian::native, T, 1>(begin, end);
}

template<std::integral T, std::size_t N = sizeof(T)>
constexpr std::optional<T>
next(byte_range auto& range) {
	return next(std::ranges::begin(range), std::ranges::end(range));
}

#include <iterator>

template<std::input_iterator It>
struct bytes {
	bytes_visitor_iterator<It> m_begin;
	bytes_visitor_iterator<It> m_end;

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