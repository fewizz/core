#pragma once

#include <bits/stdint-uintn.h>
#include <iterator>
#include <stdexcept>
#include <stdint.h>
#include <tl/expected.hpp>
#include <type_traits>
#include <utility>
#include "utf8.hpp"
#include "utf16.hpp"
#include "request_error.hpp"

namespace enc {

/*template<class D>
struct encoding_base {
	static constexpr auto char_width(auto& range) {
		return D::char_width(
			util::bytes_visitor_iterator{ std::ranges::begin(range) },
			util::bytes_visitor_iterator{ std::ranges::end(range) }
		);
	}

	static constexpr auto character(auto& range) {
		return D::character(
			util::bytes_visitor_iterator{ std::ranges::begin(range) },
			util::bytes_visitor_iterator{ std::ranges::end(range) }
		);
	}
};*/

struct utf8 {
	static constexpr int preferred_size = 1;
	
	static constexpr auto size(auto begin, auto end) {
		return util::utf8::size(begin, end);
	}

	static constexpr auto character(auto begin, auto end) {
		return util::utf8::character(begin, end);
	}
};

struct utf16 {
	static constexpr int preferred_size = 2;
	
	static constexpr auto size(auto begin, auto end) {
		return util::utf16::size(begin, end);
	}

	static constexpr auto character(auto begin, auto end) {
		return util::utf16::character(begin, end);
	}
};

struct ascii {
	static constexpr int preferred_size = 1;
	static constexpr int fixed_size = 1;

	static constexpr tl::expected<uint8_t, request_error>
	size(auto begin, auto end) {
		return { 1 };
	}

	static constexpr tl::expected<uint64_t, request_error>
	codepoint(util::byte_iterator auto begin, util::byte_iterator auto end) {
		return { (uint64_t) *begin };
	}
};

template<class E>
static constexpr auto size(auto& range) {
	auto begin = std::ranges::begin(range);
	using It = decltype(begin);

	return E::size(
		util::bytes_visitor_iterator< It, std::endian::big > { begin },
		util::bytes_visitor_iterator< It, std::endian::big > { std::ranges::end(range) }
	).value();
}

template<class E, class It>
static constexpr auto size(It begin, It end) {
	return E::size(
		util::bytes_visitor_iterator< It, std::endian::big > { begin },
		util::bytes_visitor_iterator< It, std::endian::big > { end }
	).value();
}

template<class E>
static constexpr auto codepoint(auto& range) {
	auto begin = std::ranges::begin(range);
	using It = decltype(begin);

	return E::character(
		util::bytes_visitor_iterator< It, std::endian::big > { begin },
		util::bytes_visitor_iterator< It, std::endian::big > { std::ranges::end(range) }
	).value().codepoint;
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
	{ T::preferred_size };
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
