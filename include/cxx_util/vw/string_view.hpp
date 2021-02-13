#pragma once

//#include "common.hpp"
#include "../encoding/encoding.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <compare>
#include <cstring>
#include <span>
#include <stdexcept>
#include <type_traits>

namespace vw {

template<enc::encoding E, class T>
struct string_view_base : std::span<T> {
	using base_type = std::span<T>;
	using value_type = character_view<E, T>;
	using iterator = character_iterator<E, T>;
	using typename base_type::size_type;
	static constexpr size_type npos = size_type(-1);

	constexpr string_view_base() = default;

	template<class It>
	constexpr string_view_base(It begin, size_type count)
		: base_type(begin, count) {}

	constexpr string_view_base(iterator begin, iterator end)
		: base_type(begin.cur, end.cur - begin.cur) {}

	constexpr string_view_base(iterator begin, size_type count)
		: string_view_base(begin, begin + count) {}

	using base_type::data;

	constexpr iterator begin() const {
		return { data(), data(), data() + base_type::size() };
	}

	constexpr iterator end() const {
		return { data(), data() + base_type::size() };
	}

	constexpr size_type size() const {
		return std::distance(begin(), end());
	}

	constexpr size_type length() const {
		return size();
	}

	constexpr bool empty() const { return size() == 0; }

	constexpr value_type operator [] (size_type pos) const {
		auto it = begin();
		std::advance(it, pos);
		return *it;
	}

	constexpr value_type at(size_type pos) const {
		if(pos >= size()) throw std::runtime_error{ "vw::string_view::at" };
		auto it = begin();
		std::advance(it, pos);
		return *it;
	}

	constexpr value_type front() const {
		return *begin();
	}

	constexpr value_type back() const {
		auto it = begin();
		std::advance(it, size() - 1);
		return *it;
	}

	constexpr string_view_base substr(size_type pos = 0, size_type n = npos) const {
		auto b = begin();
		std::advance(b, pos);

		return { b, n == npos ? size() - pos : n };
	}

	constexpr bool operator == (const string_view_base& that) const {
		return that.size() == size() && std::__memcmp(data(), that.data(), size()) == 0;
	};

	constexpr size_type find(value_type s, size_type pos = 0) const {
		
	}

};

template<class E, class T = util::uint_with_size_t<sizeof(E::preferred_size)>>
struct basic_string_view : string_view_base<E, T> {
	using string_view_base<E, T>::string_view_base;
};

template<class T>
struct basic_string_view<enc::ascii, T> : string_view_base<enc::ascii, T> {
	using base_type = string_view_base<enc::ascii, T>;

	constexpr basic_string_view(const char* str)
	requires(std::is_same_v<T, const char>)
		: base_type(
			str,
			std::string_view(str).length()
		) {}
};

template<class T>
struct basic_string_view<enc::utf8, T> : string_view_base<enc::utf8, T> {
	using base_type = string_view_base<enc::utf8, T>;

	constexpr basic_string_view(const char8_t* str)
	requires(std::is_same_v<T, const char8_t>)
		: base_type(
			str,
			std::u8string_view(str).length()
		) {}
};

using ascii_string_view = basic_string_view<enc::ascii, const char>;
using utf8_string_view = basic_string_view<enc::utf8, const char8_t>;

}