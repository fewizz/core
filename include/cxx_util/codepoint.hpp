#pragma once

#include "character_set.hpp"
#include <concepts>
#include <compare>

namespace u {

template<character_set CS>
struct codepoint {
	using base_type = typename CS::codepoint_base_type;

	static_assert(std::unsigned_integral<base_type>);
private:
	base_type m_value;
public:
	using character_set_type = CS;

	constexpr codepoint() = default;
	constexpr codepoint(base_type v) : m_value{v} {}
	constexpr codepoint& operator = (base_type v) {
		m_value = v;
		return *this;
	}

	constexpr std::strong_ordering
	operator <=> (const codepoint& cp) const = default;

	constexpr bool
	operator == (const codepoint& cp) const = default;

	constexpr std::strong_ordering
	operator <=> (std::signed_integral auto cp) const {
		if(cp < 0) return std::strong_ordering::greater;
		return m_value <=> uintmax_t(cp);
	};

	constexpr bool
	operator == (std::integral auto cp) const {
		return (*this <=> cp) == 0;
	};

	constexpr base_type value () const {
		return m_value;
	}
};

}