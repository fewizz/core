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
#include "byte.hpp"
#include "encoding/request_error.hpp"
#include <climits>

static_assert( CHAR_BIT == 8 );

namespace u {

template<class It, std::endian Endian>
class byte_reference {
	using iter_value_type = std::iter_value_t<It>;
	constexpr static unsigned iter_value_size = sizeof(iter_value_type);
	It m_base;
	unsigned m_byte_index = 0;

public:

	constexpr byte_reference() = default;
	constexpr byte_reference(It base, unsigned byte_index)
		: m_base { base }, m_byte_index{ byte_index } {}

	constexpr byte_reference& operator = (std::byte value) {
		u::write_byte<Endian>(
			value,
			*m_base,
			m_byte_index
		);

		return *this;
	}

	constexpr operator std::byte () const {
		return u::read_byte<Endian>(*m_base, m_byte_index);
	}

	constexpr auto operator <=> (std::byte b) const {
		return *m_base <=> b;
	}
};

template<class It, std::endian Endian = std::endian::little>
struct byte_iterator {
private:
	It m_base;
	unsigned m_byte_index = 0;

public:
	using base_iterator_value_type = std::iter_value_t<It>;
	constexpr static unsigned base_iterator_value_type_size
		= sizeof( base_iterator_value_type );

	using value_type = byte_reference<It, Endian>;
	using difference_type = std::ptrdiff_t;
	//using iterator_category = std::input_iterator_tag;

	using base_iterator_category = typename std::iterator_traits<It>::iterator_category;

	static constexpr bool base_is_random_access_iterator =
		std::derived_from<base_iterator_category, std::random_access_iterator_tag>;

	constexpr byte_iterator() {}
	constexpr byte_iterator(It iter) : m_base { iter } {}

protected:
	constexpr byte_iterator(It iter, unsigned byte_index)
		: m_base { iter }, m_byte_index { byte_index } {}

public:
	constexpr auto base() const { return m_base; }

	constexpr value_type
	operator * () const {
		return { m_base, m_byte_index };
	}

	constexpr byte_iterator&
	operator ++ () {
		++m_byte_index;

		if(m_byte_index >= base_iterator_value_type_size) {
			++m_base;
			m_byte_index = 0;
		}

		return *this;
	}

	constexpr byte_iterator&
	operator -- () {
		if(m_byte_index == 0) {
			--m_base;
			m_byte_index = base_iterator_value_type_size;
		}

		--m_byte_index;

		return *this;
	}

	constexpr byte_iterator
	operator ++ (int) {
		byte_iterator prev = *this;
		++(*this);
		return prev;
	}

	constexpr byte_iterator
	operator -- (int) {
		byte_iterator prev = *this;
		--(*this);
		return prev;
	}

	constexpr byte_iterator
	operator + (difference_type n) const
	requires( base_is_random_access_iterator ) {
		byte_iterator copy = *this;
		return copy += n;
	}

	constexpr byte_iterator
	operator - (difference_type n) const
	requires( base_is_random_access_iterator ) {
		byte_iterator copy = *this;
		return copy -= n;
	}

	constexpr difference_type
	operator - (const byte_iterator& that) const 
	requires( base_is_random_access_iterator ) {
		return (m_base - that.iter) * base_iterator_value_type_size + (m_byte_index - that.byte_index);
	}

	constexpr byte_iterator&
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

		auto n0 = n + m_byte_index;
		auto [iter_adv, rem] = div(n0, base_iterator_value_type_size);

		std::advance(m_base, iter_adv);
		m_byte_index = rem;

		return *this;
	}

	constexpr byte_iterator&
	operator -= (difference_type n)
	requires( base_is_random_access_iterator ) {
		return *this += -n;
	}

	constexpr std::byte
	operator [] (std::ptrdiff_t n) 
	const requires( base_is_random_access_iterator ) {
		return *this + n;
	}

	auto operator <=> (const byte_iterator& that) const = default;
};

template<class BaseIt>
constexpr auto operator + (std::ptrdiff_t n, const byte_iterator<BaseIt>& that) {
	return that + n;
}

template<class It>
concept input_iterator_of_type_convertible_to_byte =
	std::input_iterator<It> &&
	std::is_convertible_v<std::iter_value_t<It>, std::byte>;

template<class T>
struct is_input_iterator_of_type_convertible_to_byte : std::false_type {};

template<input_iterator_of_type_convertible_to_byte T>
struct is_input_iterator_of_type_convertible_to_byte<T> : std::true_type {};

template<class R>
concept range_of_type_convertible_to_byte =
	std::ranges::range<R> &&
	std::is_convertible_v<std::ranges::range_value_t<R>, std::byte>;

template<std::endian E, std::integral T, std::size_t N = sizeof(T)>
constexpr std::optional<T>
next(
	u::input_iterator_of_type_convertible_to_byte auto& begin,
	u::input_iterator_of_type_convertible_to_byte auto end
) {
	auto dist = std::distance(begin, end);

	if(dist < N) return { };

	T result = T(std::byte(*begin));
	++begin;
	
	for(std::size_t i = 1; i < N; i++) {
		if constexpr (E == std::endian::little) {
			result |= T(std::byte(*begin)) << ( 8 * i );
		}
		else {
			result <<= 8;
			result |= T(std::byte(*begin));
		}
		++begin;
	}

	return { result };
}

template<std::integral T> requires(sizeof(T) == 1)
constexpr std::optional<T>
next(auto& begin, auto end) {
	return next<std::endian::native, T, 1>(begin, end);
}

/*template<std::integral T, std::size_t N = sizeof(T)>
constexpr std::optional<T>
next(auto& range) {
	return next(std::ranges::begin(range), std::ranges::end(range));
}*/

#include <iterator>

template<std::input_iterator It>
struct bytes {
	byte_iterator<It> m_begin;
	byte_iterator<It> m_end;

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