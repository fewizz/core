#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "int.hpp"
#include "bit.hpp"
#include "byte_range.hpp"
#include "codec.hpp"
#include "codepoint.hpp"
#include "object.hpp"

namespace u {

template<
	u::codec C,
	class It
>
struct encoded_character_view {
private:
	It m_begin;
	It m_end;
public:
	using size_type = std::size_t;

	constexpr encoded_character_view() = default;
	constexpr encoded_character_view(const encoded_character_view& ch) = default;
	constexpr encoded_character_view(encoded_character_view&& base) = default;
	constexpr encoded_character_view(It begin, It end)
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

	u::codepoint<C> codepoint() {
		return C::decoder_type::convert(begin());
	}

	template<std::ranges::range R>
	auto operator <=> (const R& range) const {
		return
			u::byte_range { *this }
			<=>
			u::byte_range { range };
	}

	auto operator <=> (const auto& obj) const {
		return *this <=> (u::obj_representation_reference{ obj });
	}

	auto operator <=> (const u::codepoint<C>& cp) const {
		codepoint() <=> cp;
	}

	bool operator == (const auto& v) const {
		return (*this <=> v) == 0;
	}
};

}