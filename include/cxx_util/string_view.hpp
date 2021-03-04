#pragma once

#include "character.hpp"
#include "character_iterator.hpp"
#include <algorithm>
#include <compare>
#include <cstring>
#include <iterator>
#include <span>
#include <stdexcept>
#include <type_traits>
#include "string_common_base.hpp"
#include <string_view>

namespace u {

template<enc::encoding E, class It>
struct basic_string_view
: internal::string_common_base<
	basic_string_view<E, It>,
	E,
	It
> {
	using base_type = internal::string_common_base<
		basic_string_view<E, It>,
		E,
		It
	>;

	using typename base_type::base_iterator_value_type;
	using typename base_type::size_type;
	using typename base_type::value_type;
	using typename base_type::iterator;
	using base_type::npos;

private:
	It m_begin;
	It m_end;
public:

	constexpr basic_string_view() = default;

	constexpr basic_string_view(It begin, It end)
		: m_begin{ begin}, m_end{ end } {}

	constexpr basic_string_view(It begin, size_type count)
		: basic_string_view(begin, begin + count) {}

	constexpr basic_string_view(iterator begin, iterator end)
		: basic_string_view(begin.base(), end.base()) {}

	template<std::ranges::range R>
	constexpr basic_string_view(R& range)
		:
		m_begin{ std::ranges::begin(range) },
		m_end{ std::ranges::end(range) } {}

	constexpr basic_string_view(const base_iterator_value_type* c_str) {
		m_begin = c_str;
		m_end = c_str
			+ std::basic_string_view<
				base_iterator_value_type
			>{ c_str }.size();
	}

	auto raw_begin() const {
		return m_begin;
	}

	auto raw_end() const {
		return m_end;
	}

	using base_type::front;
	using base_type::at;
	using base_type::operator [];
};

//using ascii_string_view = basic_string_view<enc::ascii, const char>;
//using u8_string_view = basic_string_view<enc::utf8, const char8_t>;

}