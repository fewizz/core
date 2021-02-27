#pragma once

#include <bits/stdint-uintn.h>
#include <compare>
#include <iterator>
#include <stdexcept>
#include <stdint.h>
#include <tl/expected.hpp>
#include <type_traits>
#include <utility>
#include "request_error.hpp"
#include "../bytes.hpp"
#include "../convert.hpp"

namespace enc {

template<class CS>
struct codepoint {
	using character_set_type = CS;
	using base_type = typename CS::codepoint_type;
	base_type m_value;

	constexpr codepoint() = default;
	constexpr codepoint(base_type v) : m_value{v} {}
	constexpr codepoint& operator = (base_type v) {
		m_value = v;
		return *this;
	}

	constexpr std::strong_ordering
	operator <=> (const codepoint&) const = default;

	constexpr std::strong_ordering
	operator <=> (base_type cp) const {
		return m_value <=> cp;
	};

	constexpr bool operator == (base_type cp) const {
		return cp == m_value;
	};

	constexpr codepoint operator >> (const auto& b) const {
		return { m_value >> b };
	}

	constexpr codepoint operator << (const auto& b) const {
		return { m_value << b };
	}

	constexpr codepoint operator & (const auto& b) const {
		return { m_value & b };
	}

	constexpr codepoint operator | (const auto& b) const {
		return { m_value | b };
	}
};

template<class CS>
struct codepoint_read_result {
	codepoint<CS> codepoint;
	uint8_t width;
};

template<class CS>
struct codepoint_write_result {
	uint8_t code_units;
};

template<class CP>
requires(
	std::is_same_v<
		codepoint<typename CP::character_set_type>,
		CP
	>
)
constexpr CP convert_to(CP cp) {
	return cp;
}

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
constexpr codepoint<typename E::character_set_type>
read_codepoint(auto& range) {
	return E::read(
		u::byte_iterator { std::ranges::begin(range) },
		u::byte_iterator { std::ranges::end(range) }
	).value().codepoint;
}

template<class T>
concept encoding = requires {
	typename T::character_set_type;
};

template<class T>
concept fixed_size_encoding = encoding<T> && requires() {
	{ T::size };
};

template<class T>
struct is_encoding : std::false_type {}; 

template<encoding T>
struct is_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_encoding_v = is_encoding<T>::value;

template<class T>
struct is_fixed_width_encoding : std::false_type {}; 

template<encoding T>
struct is_fixed_width_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_fixed_width_encoding_v = is_fixed_width_encoding<T>::value;

}
