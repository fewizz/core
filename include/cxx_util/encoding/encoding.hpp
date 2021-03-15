#pragma once

#include <compare>
#include <iterator>
#include <stdexcept>
#include <stdint.h>
#include <tl/expected.hpp>
#include <type_traits>
#include <utility>
#include "request_error.hpp"
#include "../byte_iterator.hpp"

namespace enc {

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

template<class CS>
struct codepoint_read_result {
	enc::codepoint<CS> codepoint;
	uint8_t width;
};

template<class CS>
struct codepoint_write_result {
	uint8_t width;
};

template<class E>
static constexpr auto size(auto& range) {
	return E::size(
		u::byte_iterator { std::ranges::begin(range) },
		u::byte_iterator { std::ranges::end(range) }
	).value();
}

template<class E, class It>
static constexpr auto size(It begin, It end) {
	return E::size(
		u::byte_iterator { begin },
		u::byte_iterator { end }
	).value();
}

template<class E>
codepoint<typename E::character_set_type>
read_codepoint(auto&& range) {
	return E::read(
		u::byte_iterator { std::ranges::begin(range) },
		u::byte_iterator { std::ranges::end(range) }
	).value().codepoint;
}

template<class E>
void
write_codepoint(auto cp, auto&& range) {
	E::write(
		cp,
		u::byte_iterator { std::ranges::begin(range) },
		u::byte_iterator { std::ranges::end(range) }
	);
}

}
