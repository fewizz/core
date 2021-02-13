#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "../encoding/encoding.hpp"
//#include "../encoding/convert.hpp"
#include "../int.hpp"

namespace vw {

template<
	enc::encoding Encoding,
	class T
>
struct character_view : std::span<T> {
	using base_type = std::span<T>;
	using typename base_type::size_type;

	constexpr character_view() = default;
	constexpr character_view(const character_view& ch) = default;
	constexpr character_view(character_view&& base) = default;
	constexpr character_view(const T* begin, size_type count) : base_type(begin, count) {};

	using base_type::data;
	using base_type::size;

	constexpr auto operator <=> (const character_view& that) const {
		return
			enc::codepoint<Encoding>(*this)
			<=>
			enc::codepoint<Encoding>(that)
		;
	}

	constexpr bool operator == (const character_view& that) const = default;

	constexpr auto operator <=> (const auto& range) const {
		return
			enc::codepoint<Encoding>(*this)
			<=>
			enc::codepoint<Encoding>(range)
		;
	}

	constexpr bool operator == (const auto& range) const {
		return (*this <=> range) == 0;
	}
};

}