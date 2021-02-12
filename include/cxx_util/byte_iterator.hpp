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

template<class Iter, std::endian Endian = std::endian::little>
struct bytes_visitor_iterator {
	using prev_value_type = std::iter_value_t<Iter>;
	constexpr static unsigned prev_value_type_size = sizeof(prev_value_type);

	using value_type = std::byte;
	using difference_type = int;
	using reference = std::byte&;
	using pointer = std::byte*;
	using iterator_category = std::random_access_iterator_tag;

	Iter iter;
	unsigned byte_index = 0;

	constexpr bytes_visitor_iterator() {}
	constexpr bytes_visitor_iterator(Iter iter) : iter{iter} {}

protected:
	constexpr bytes_visitor_iterator(Iter iter, unsigned byte_index)
		: iter{iter}, byte_index{byte_index} {}

public:
	constexpr std::byte operator * () const {
		return util::byte(
			*iter,
			Endian == std::endian::little ?
				byte_index :
				prev_value_type_size - byte_index - 1
		);
	}

	constexpr bytes_visitor_iterator& operator += (difference_type n) {
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

		std::advance(iter, iter_adv);
		byte_index = rem;

		return *this;
	}

	constexpr bytes_visitor_iterator& operator -= (difference_type n) {
		return *this += -n;
	}

	constexpr bytes_visitor_iterator& operator ++ () {
		return *this += 1;
	}

	constexpr bytes_visitor_iterator& operator -- () {
		return *this += -1;
	}

	constexpr bytes_visitor_iterator operator ++ (int) {
		bytes_visitor_iterator prev = *this;
		++(*this);
		return prev;
	}

	constexpr bytes_visitor_iterator operator -- (int) {
		bytes_visitor_iterator prev = *this;
		--(*this);
		return prev;
	}

	constexpr bytes_visitor_iterator operator + (difference_type n) const {
		bytes_visitor_iterator copy = *this;
		return copy += n;
	}

	constexpr bytes_visitor_iterator operator - (difference_type n) const {
		bytes_visitor_iterator copy = *this;
		return copy -= n;
	}

	constexpr difference_type operator - (const bytes_visitor_iterator& that) const {
		return (iter - that.iter) * prev_value_type_size + (byte_index - that.byte_index);
	}

	auto operator <=> (const bytes_visitor_iterator& that) const = default;
};

template<class R>
concept byte_range =
	std::ranges::range<R> &&
	std::is_same_v<std::ranges::range_value_t<R>, std::byte>;

template<class It>
concept byte_iterator =
	std::input_iterator<It> && std::same_as<std::iter_value_t<It>, std::byte>;

template<std::integral T, std::size_t N = sizeof(T)>
constexpr std::optional<T>
next(byte_iterator auto& begin, byte_iterator auto end) {
	std::size_t dist = std::distance(begin, end);

	if(dist < N) return { };

	T res = T(*begin);
	++begin;
	
	for(std::size_t i = 1; i < N; i++) {
		res <<= 8;
		res |= T(*begin);
		++begin;
	}

	return { res };
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
	constexpr auto size() const { return m_end - m_begin; }
};

/*template<class R>
constexpr auto begin(const bytes_visitor_range<R>& r) { return r.m_begin; }

template<class R>
constexpr auto end(const bytes_visitor_range<R>& r) { return r.m_end; }*/

}