#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "../encoding/encoding.hpp"
//#include "../encoding/convert.hpp"
#include "../int.hpp"
#include "../bit.hpp"

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
	constexpr character_view(T* begin, size_type count) : base_type(begin, count) {};

	using base_type::data;
	using base_type::size;
	using base_type::begin;
	using base_type::end;

	constexpr auto operator <=> (const auto& range) const {
		return
			util::bytes { *this }
			<=>
			util::bytes { range };
	}

	constexpr bool operator == (const auto& range) const {
		return (*this <=> range) == 0;
	}
};

}