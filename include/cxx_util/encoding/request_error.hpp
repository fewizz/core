#pragma once

#include <bits/stdint-uintn.h>
#include <codecvt>
#include <optional>
#include <stdint.h>

namespace enc {

struct character {
	uint64_t codepoint;
	uint8_t width;
};

struct character_builder {
	std::optional<uint64_t> m_codepoint;
	std::optional<uint8_t> m_width;

	constexpr character_builder() = default;

	constexpr character_builder& codepoint(uint64_t v) {
		m_codepoint = v;
		return *this;
	}

	constexpr character_builder& width(uint8_t v) {
		m_width = v;
		return *this;
	}

	constexpr operator character() { return { *m_codepoint, *m_width }; }
};

enum class request_error {
	preconditions = 1,
	unexpected_src_end = 2,
	invalid_input = 3
};

}