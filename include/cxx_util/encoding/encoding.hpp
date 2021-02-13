#pragma once

#include <bits/stdint-uintn.h>
#include <iterator>
#include <stdexcept>
#include <stdint.h>
#include <tl/expected.hpp>
#include <type_traits>
#include <utility>
#include "request_error.hpp"
#include "../byte_iterator.hpp"

namespace enc {

struct unicode {
	using code_point_type = uint32_t;
};

using ucs = unicode;

template<class CS>
struct character {
	typename CS::code_point_type code_point;
	uint8_t width;
};

template<class CS>
struct character_builder {
	using code_point_type = typename CS::code_point_type;
	using character_type = enc::character<CS>;
	std::optional<code_point_type> m_codepoint;
	std::optional<uint8_t> m_width;

	constexpr character_builder() = default;

	constexpr character_builder& codepoint(code_point_type v) {
		m_codepoint = v;
		return *this;
	}

	constexpr character_builder& width(uint8_t v) {
		m_width = v;
		return *this;
	}

	constexpr operator character_type()
	{ return { *m_codepoint, *m_width }; }
};

struct ascii;

struct ascii {
	using code_point_type = uint8_t;
	using character_set_type = ascii;

	static constexpr int code_unit_bits = 7;
	static constexpr int code_unit_size = 1;

	static constexpr tl::expected<uint8_t, request_error>
	size(auto begin, auto end) {
		return { 1 };
	}

	static constexpr tl::expected<character<ascii>, request_error>
	decode(util::byte_iterator auto begin, util::byte_iterator auto end) {
		return character_builder<ascii>{}.codepoint((uint64_t) *begin).width(1);
	}
};

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
static constexpr auto codepoint(auto& range) {
	return E::decode(
		util::bytes_visitor_iterator { std::ranges::begin(range) },
		util::bytes_visitor_iterator { std::ranges::end(range) }
	).value().code_point;
}

/*struct utf16 {
	static constexpr int preferred_size = 2;

	static constexpr auto char_width(const auto& range) {
		return util::utf16::char_width(range);
	}

	static constexpr auto codepoint(const auto& range) {
		return util::utf16::codepoint(range);
	}
};
*/

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
concept encoding = requires() {
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
