#pragma once

#include <bits/c++config.h>
#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "encoding/encoding.hpp"
#include "int.hpp"
#include "bit.hpp"
#include "byte_range.hpp"

namespace u {

template<
	enc::encoding Encoding,
	class It
>
struct character_view {
private:
	It m_begin;
	It m_end;
public:
	using size_type = std::size_t;

	constexpr character_view() = default;
	constexpr character_view(const character_view& ch) = default;
	constexpr character_view(character_view&& base) = default;
	constexpr character_view(It begin, It end)
	: m_begin{ begin }, m_end{ end } {};

	size_type size() const {
		return std::distance(m_begin, m_end);
	}

	It begin() const {
		return m_begin;
	};

	It end() const {
		return m_end;
	};

	constexpr auto operator <=> (const auto& range) const {
		return
			u::byte_range { *this }
			<=>
			u::byte_range { range };
	}

	constexpr bool operator == (const auto& range) const {
		return (*this <=> range) == 0;
	}
};

}