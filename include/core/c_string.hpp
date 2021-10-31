#pragma once

#include "integer.hpp"
#include "type/remove_const.hpp"
#include "types/are_same.hpp"
#include "span.hpp"

struct null_characted_sentinel {};

constexpr bool operator == (null_characted_sentinel s, const char* ptr) {
	return *ptr == 0;
}

struct c_string {
	using value_type = char;

	const char* m_ptr;

	constexpr const char* begin() const { return m_ptr; }
	constexpr null_characted_sentinel end() const { return {}; }
};