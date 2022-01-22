#pragma once

#include "span.hpp"

struct null_characted_sentinel {};

constexpr bool operator == (null_characted_sentinel s, const char* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const char* ptr, null_characted_sentinel s) {
	return *ptr == 0;
}

struct c_string {
	using value_type = char;

	const char* m_ptr;

	constexpr const char* begin() const { return m_ptr; }
	constexpr null_characted_sentinel end() const { return {}; }
};

constexpr bool operator == (c_string a, c_string b) {
	auto abegin = a.begin();
	auto aend = a.begin();
	auto bbegin = b.begin();
	auto bend = b.begin();

	while(abegin != aend && bbegin != bend && *abegin == *bbegin) {
		++abegin;
		++bbegin;
	}

	return abegin == aend && bbegin == bend;
}