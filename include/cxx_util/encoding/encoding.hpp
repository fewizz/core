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
#include "../byte_iterator.hpp"
#include "../convert.hpp"

namespace enc {

struct unicode {
	using codepoint_type = uint32_t;
};

using ucs = unicode;

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

	//constexpr operator base_type () { return m_value; }
};

template<class CS>
struct codepoint_parse_result {
	codepoint<CS> codepoint;
	uint8_t width;
};

/*template<class CS>
struct character_builder {
	using codepoint_type = typename CS::codepoint_type;
	using character_type = enc::codepoint_request_result<CS>;
	std::optional<codepoint_type> m_codepoint;
	std::optional<uint8_t> m_width;

	constexpr character_builder() = default;

	constexpr character_builder& codepoint(codepoint_type v) {
		m_codepoint = v;
		return *this;
	}

	constexpr character_builder& width(uint8_t v) {
		m_width = v;
		return *this;
	}

	constexpr operator character_type()
	{ return { *m_codepoint, *m_width }; }
};*/

/*template<class CS>
struct codepoint_from {
	template<class ToCS>
	typename ToCS::codepoint_type to(typename CS::codepoint_type cp);
};*/

struct ascii {
	using codepoint_type = uint8_t;
	using character_set_type = ascii;

	static constexpr int code_unit_bits = 7;
	static constexpr int code_unit_size = 1;

	static constexpr tl::expected<uint8_t, request_error>
	size(auto begin, auto end) {
		return { 1 };
	}

	static constexpr tl::expected<codepoint_parse_result<ascii>, request_error>
	read(util::byte_iterator auto begin, util::byte_iterator auto end) {
		codepoint_type possible = (uint8_t) *begin;

		if(possible >= 0x80) return tl::unexpected { request_error::invalid_input };

		return codepoint_parse_result<ascii>{ possible, 1 };
	}
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

template<std::same_as<codepoint<unicode>>>
constexpr codepoint<unicode> convert_to(codepoint<ascii> cp) {
	return cp.m_value;
}

template<std::same_as<codepoint<ascii>>>
constexpr codepoint<ascii> convert_to(codepoint<unicode> cp) {
	if(cp.m_value > 0xFF) throw std::runtime_error{ "" };
	return cp.m_value;
}

static_assert(util::is_convertible_to_v<codepoint<ascii>, codepoint<unicode>>);
static_assert(util::is_convertible_to_v<codepoint<unicode>, codepoint<ascii>>);

static_assert(util::is_convertible_to_v<codepoint<unicode>, codepoint<unicode>>);
static_assert(util::is_convertible_to_v<codepoint<ascii>, codepoint<ascii>>);

template<class E>
static constexpr auto size(auto& range) {
	return E::size(
		util::bytes_visitor_iterator { std::ranges::begin(range) },
		util::bytes_visitor_iterator { std::ranges::end(range) }
	).value();
}

template<class E, class It>
static constexpr auto size(It begin, It end) {
	return E::size(
		util::bytes_visitor_iterator { begin },
		util::bytes_visitor_iterator { end }
	).value();
}

template<class E>
constexpr codepoint<typename E::character_set_type>
read_codepoint(auto& range) {
	return E::read(
		util::bytes_visitor_iterator { std::ranges::begin(range) },
		util::bytes_visitor_iterator { std::ranges::end(range) }
	).value().codepoint;
}

/*template<class E>
static constexpr auto codepoint(auto& range) {
	return E::decode(
		util::bytes_visitor_iterator { std::ranges::begin(range) },
		util::bytes_visitor_iterator { std::ranges::end(range) }
	).value().code_point;
}*/

/*struct usc2 {
	using char_type = char16_t;
	static constexpr int characters = 2;

	static size_request_info first_char_width(const char_type* begin, const char_type* end) {
		if(begin > end) return { enc::request_result::preconditions };
		if(std::distance(begin, end) < 2) return { enc::request_result::unexpected_src_end };
		return { enc::request_result::ok, 2 };
	}

	static codepoint_request_info first_codepoint(const char_type* begin, const char_type* end) {
		if(begin > end) return { enc::request_result::preconditions };
		if(std::distance(begin, end) < 1) return { enc::request_result::unexpected_src_end };
		return { enc::request_result::ok, (uint64_t) begin[0] };
	}
};*/

template<class T>
concept encoding = requires {
	/*{ T::width };
	{ T::codepoint };*/
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

/*template<class CharT>
struct encoding_from_char;

template<> struct encoding_from_char<char> { using type = enc::ascii; };
//template<> struct encoding_from_char<wchar_t> { using type = enc::usc2; };
template<> struct encoding_from_char<char8_t> { using type = enc::utf8; };
template<> struct encoding_from_char<char16_t> { using type = enc::utf16; };

template<class CharT>
using encoding_from_char_t = typename encoding_from_char<CharT>::type;*/

}
