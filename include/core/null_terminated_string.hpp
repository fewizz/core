#pragma once

#include "span.hpp"

struct null_character_sentinel {};

constexpr bool operator == (null_character_sentinel, const auto* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const auto* ptr, null_character_sentinel) {
	return *ptr == 0;
}

template<typename Type>
struct null_terminated_string {
	using value_type = Type;

	const value_type* m_ptr;

	constexpr const value_type* begin() const { return m_ptr; }
	constexpr null_character_sentinel end() const { return {}; }
};

template<typename Type>
constexpr bool operator == (
	null_terminated_string<Type> a,
	null_terminated_string<Type> b
) {
	auto a_begin = a.begin();
	auto a_end = a.begin();
	auto b_begin = b.begin();
	auto b_end = b.begin();

	while(a_begin != a_end && b_begin != b_end && *a_begin == *b_begin) {
		++a_begin;
		++b_begin;
	}

	return a_begin == a_end && b_begin == b_end;
}