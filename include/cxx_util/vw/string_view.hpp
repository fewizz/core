#pragma once

//#include "common.hpp"
#include "../encoding/encoding.hpp"
#include "../encoding/utf8.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <algorithm>
#include <bits/c++config.h>
#include <bits/ranges_algo.h>
#include <compare>
#include <cstring>
#include <iterator>
#include <span>
#include <stdexcept>
#include <type_traits>
#include "common.hpp"

namespace vw {

template<enc::encoding E, class T>
struct basic_string_view : std::span<T>, internal::string_common<basic_string_view<E, T>, E, T> {
	using base_type = std::span<T>;
	using common_type = internal::string_common<basic_string_view<E, T>, E, T>;

	using typename common_type::size_type;
	using typename common_type::value_type;
	using typename common_type::iterator;

	using common_type::npos;

	constexpr basic_string_view() = default;

	template<class It>
	constexpr basic_string_view(It begin, size_type count)
		: base_type(begin, count) {}

	constexpr basic_string_view(iterator begin, iterator end)
		: base_type(begin.cur, end.cur - begin.cur) {}

	template<class R>
	constexpr basic_string_view(R&& range)
		: base_type(std::forward<R>(range)) {}

	constexpr basic_string_view(T* c_str)
		: base_type(
			std::basic_string_view { c_str }
		) {}

	using base_type::data;

	constexpr size_type raw_size() const {
		return base_type::size();
	}

	using common_type::size;
	using common_type::front;
	using common_type::back;
	using common_type::at;
	using common_type::operator [];
};

using ascii_string_view = basic_string_view<enc::ascii, const char>;
using utf8_string_view = basic_string_view<enc::utf8, const char8_t>;

}