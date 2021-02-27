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

template<class CarSet>
struct codepoint {
	using base_type = typename CarSet::codepoint_type;

	static_assert(std::unsigned_integral<base_type>);
private:
	base_type m_value;
public:
	using character_set_type = CarSet;

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

template<class Codepoint>
requires(
	std::is_same_v<
		codepoint<typename Codepoint::character_set_type>,
		Codepoint
	>
)
constexpr Codepoint convert_to(Codepoint cp) {
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
