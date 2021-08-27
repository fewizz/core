#pragma once

#include "encoding.hpp"
#include "ascii.hpp"
#include "unicode.hpp"

#include "../convert.hpp"

namespace enc {

template<std::same_as<codepoint<ascii>>>
constexpr codepoint<ascii> convert_to(codepoint<unicode> cp) {
	if(cp.m_value > 0xFF) throw std::runtime_error{ "" };
		return cp.m_value;
}

static_assert(u::is_convertible_to_v<codepoint<ascii>, codepoint<unicode>>);

}