#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "encoding/encoding.hpp"
#include "int.hpp"
#include "bit.hpp"
#include "byte_range.hpp"
#include "obj_representation.hpp"

namespace u {

template<
	enc::encoding E,
	class It
>
struct character_view {
private:
	It m_begin;
	It m_end;
public:
	using size_type = std::size_t;
	using character_set_type = typename E::character_set_type;

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

	enc::codepoint<character_set_type> codepoint() {
		return enc::read_codepoint<E>(*this);
	}

	template<std::ranges::range R>
	auto operator <=> (const R& range) const {
		return
			u::byte_range { *this }
			<=>
			u::byte_range { range };
	}

	auto operator <=> (const auto& obj) const {
		return *this <=> (u::obj_representation{ obj });
	}

	auto operator <=> (const enc::codepoint<character_set_type>& cp) const {
		codepoint() <=> cp;
	}

	bool operator == (const auto& v) const {
		return (*this <=> v) == 0;
	}
};

}