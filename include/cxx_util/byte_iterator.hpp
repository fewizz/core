#pragma once

#include <bit>
#include <bits/c++config.h>
#include <bits/iterator_concepts.h>
#include <codecvt>
#include <cstddef>
#include <iterator>
#include <cstring>
#include <memory>
#include <tl/expected.hpp>
#include <type_traits>
#include "bit.hpp"
#include "encoding/request_error.hpp"

namespace util {

template<class It, std::endian Endian = std::endian::little>
struct bytes_visitor_iterator {
private:
	It m_base;
	unsigned byte_index = 0;

public:
	using prev_value_type = std::iter_value_t<It>;
	constexpr static unsigned prev_value_type_size = sizeof(prev_value_type);

	using value_type = std::byte;
	using difference_type = std::ptrdiff_t;

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

	constexpr std::byte
	operator * () const {
		return util::byte(
			*m_base,
			Endian == std::endian::little ?
				byte_index :
				prev_value_type_size - byte_index - 1
		);
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

template<std::ranges::range R, class It = std::ranges::iterator_t<R>>
struct bytes {
	bytes_visitor_iterator<It> m_begin;
	bytes_visitor_iterator<It> m_end;

	constexpr bytes(R& range)
	:
		m_begin{ std::ranges::begin(range) },
		m_end{ std::ranges::end(range) }
		{}

	constexpr auto begin() const { return m_begin; }
	constexpr auto end() const { return m_end; }
	constexpr auto size() const { return std::ranges::size(m_begin, m_end); }
};

}