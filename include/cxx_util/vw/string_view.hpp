#pragma once

//#include "common.hpp"
#include "../encoding/encoding.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <cstring>
#include <span>
#include <type_traits>

namespace vw {

/*namespace internal {

template<
	class Derived,
	enc::encoding E,
	class T = std::char_traits<typename E::char_type>
>
struct string_view :
internal::common<std::basic_string_view<typename E::char_type, T>, E> {
	using char_type = typename E::char_type;
	using string_view_type = std::basic_string_view<char_type, T>;
	using base_type = mb::internal::common<string_view_type, E>;
	using size_type = typename base_type::size_type;

	using base_type::base_type;

	Derived substr(size_type pos = 0, size_type count = base_type::npos) const {
		auto b = base_type::begin() + pos;
		auto e = count == base_type::npos ? base_type::end() : b + count;
		return { b, e };
	}
};

}*/

template<enc::encoding E, class T>
struct string_view_base : std::span<T> {
	using base_type = std::span<T>;
	using value_type = character_view<E, T>;
	using iterator = character_iterator<E, T>;
	using typename base_type::size_type;

	constexpr string_view_base() = default;

	template<class It>
	constexpr string_view_base(It begin, size_type count)
		: base_type(begin, count) {}

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
};

template<class E, class T = util::uint_with_size_t<sizeof(E::preferred_size)>>
struct string_view : string_view_base<E, T> {
	using string_view_base<E, T>::string_view_base;
};

template<class T>
struct string_view<enc::ascii, T> : string_view_base<enc::ascii, T> {
	using base_type = string_view_base<enc::ascii, T>;

	constexpr string_view(const char* str)
	requires(std::is_same_v<T, const char>)
		: base_type(
			str,
			std::string_view(str).length()
		) {}
};

template<class T>
struct string_view<enc::utf8, T> : string_view_base<enc::utf8, T> {
	using base_type = string_view_base<enc::utf8, T>;

	constexpr string_view(const char8_t* str)
	requires(std::is_same_v<T, const char8_t>)
		: base_type(
			str,
			std::u8string_view(str).length()
		) {}
};

using ascii_string_view = string_view<enc::ascii, const char>;
using utf8_string_view = string_view<enc::utf8, const char8_t>;

}