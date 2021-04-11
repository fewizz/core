#pragma once

#include <algorithm>
#include <bits/c++config.h>
#include <compare>
#include <cstring>
#include <iterator>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <string_view>
#include "encoded_string_iterator.hpp"
#include "utf8.hpp"
#include "ascii.hpp"
#include "codec.hpp"

namespace u {

template<u::codec C, class It>
struct encoded_string_view {
	using size_type = std::size_t;
	using value_type = u::encoded_character_view<C, It>;
	using iterator = u::encoded_string_iterator<C, It>;

private:
	It m_begin;
	It m_end;
public:

	constexpr encoded_string_view() = default;

	constexpr encoded_string_view(It begin, It end)
		: m_begin{ begin}, m_end{ end } {}

	constexpr encoded_string_view(It begin, size_type count)
		: encoded_string_view(begin, begin + count) {}

	constexpr encoded_string_view(iterator begin, iterator end)
		: encoded_string_view(begin.base(), end.base()) {}

	template<std::ranges::range R>
	constexpr encoded_string_view(R& range)
		:
		m_begin{ std::ranges::begin(range) },
		m_end{ std::ranges::end(range) } {}

	/*constexpr encoded_string_view(const * c_str) {
		m_begin = c_str;
		m_end = c_str
			+ std::basic_string_view<
				base_iterator_value_type
			>{ c_str }.size();
	}*/

	auto begin() const {
		return m_begin;
	}

	auto end() const {
		return m_begin;
	}

	auto raw_begin() const {
		return m_begin;
	}

	auto raw_end() const {
		return m_end;
	}

	constexpr bool empty() const { return raw_begin() == raw_end(); }
};

//using ascii_string_view = basic_string_view<enc::ascii, const char*>;
//using u8_string_view = basic_string_view<enc::utf8, const char8_t*>;

}