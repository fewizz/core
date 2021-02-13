#pragma once

//#include "common.hpp"
#include "../encoding/encoding.hpp"
#include "../encoding/utf8.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <compare>
#include <cstring>
#include <iterator>
#include <span>
#include <stdexcept>
#include <type_traits>

namespace vw {

template<enc::encoding E, class T>
struct basic_string_view : std::span<T> {
	using base_type = std::span<T>;
	using value_type = character_view<E, T>;
	using iterator = character_iterator<E, T>;
	
	using typename base_type::size_type;

	static constexpr size_type npos = size_type(-1);

	constexpr basic_string_view() = default;

	template<class It>
	constexpr basic_string_view(It begin, size_type count)
		: base_type(begin, count) {}

	constexpr basic_string_view(iterator begin, iterator end)
		: base_type(begin.cur, end.cur - begin.cur) {}

	template<class R>
	constexpr basic_string_view(R&& range)
		: base_type(std::forward<R>(range)) {}

	constexpr basic_string_view(const std::integral auto* c_str)
		: base_type( std::basic_string_view { c_str } ) {}

	using base_type::data;

	constexpr size_type raw_size() const {
		return base_type::size();
	}

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

	constexpr basic_string_view substr(size_type pos = 0, size_type n = npos) const {
		auto b = begin();
		std::advance(b, pos);

		if(n == npos) n = size() - pos;

		auto e = b;
		std::advance(e, n);

		return { b, e };
	}

	constexpr bool operator == (basic_string_view that) const {
		return that.size() == size() && std::__memcmp(data(), that.data(), size()) == 0;
	};

	constexpr size_type find(basic_string_view s, size_type pos = 0) const {
		auto it = begin();
		std::advance(it, pos);

		auto remains = size() - pos;
		auto that_size = s.size();

		while(remains >= that_size) {
			auto res = std::__memcmp(it.cur, s.data(), s.raw_size());
			if(res == 0) return pos;

			++pos;
			++it;
			--remains;
		}

		return npos;
	}

	constexpr size_type find(value_type c, size_type pos = 0) const {
		return find(basic_string_view { c.data(), c.size() }, pos);
	}

};
/*template<class E, class T = util::uint_with_size_t<sizeof(E::preferred_size)>>
struct basic_string_view;

template<class E, class T>
struct basic_string_view : string_view_base<basic_string_view<E, T>, E, T> {
	using base_type = string_view_base<basic_string_view<E, T>, E, T>;

	using base_type::base_type;
};*/

/*template<class T>
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
};*/

using ascii_string_view = basic_string_view<enc::ascii, const char>;
using utf8_string_view = basic_string_view<enc::utf8, const char8_t>;

}