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
#include "../bytes.hpp"

namespace enc {

template<class T>
concept character_set = requires {
	typename T::codepoint_type;
};

template<character_set CS>
struct codepoint {
	using base_type = typename CS::codepoint_type;

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

template<class CP>
struct character_set_encoding {
	using character_set_type = character_set_encoding;
	using codepoint_type = CP;

	template<u::iterator_of_bytes It>
	static constexpr tl::expected<CP, request_error>
	size(It b, It e) {
		return { sizeof(CP) };
	}

	template<u::iterator_of_bytes It>
	static tl::expected<codepoint_read_result<character_set_type>, enc::request_error>
	read(It b, It e) {
		auto cp = u::read<CP>(b, e);
		if(!cp) {
			return tl::unexpected{ enc::request_error::unexpected_src_end };
		}
		return codepoint_read_result<character_set_type> {
			cp.value(),
			sizeof(CP)
		};
	}

	template<u::iterator_of_bytes It>
	static void
	write (codepoint<character_set_type> cp, It b, It e) {
		u::write(cp, b, e);
	}
};

}
