#pragma once

#include "encoding.hpp"
#include "unicode.hpp"
#include "ascii.hpp"

namespace enc {

template<std::same_as<codepoint<unicode>>>
constexpr codepoint<unicode> convert_to(codepoint<ascii> cp) {
	return cp.m_value;
}

static_assert(u::is_convertible_to_v<codepoint<unicode>, codepoint<ascii>>);

}