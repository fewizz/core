#pragma once

#include "span.hpp"
#include "default_sentinel.hpp"

constexpr bool operator == (default_sentinel, const auto* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const auto* ptr, default_sentinel) {
	return *ptr == 0;
}

template<typename Type>
struct null_terminated_string {
	using value_type = Type;

	value_type* m_ptr;

	constexpr const value_type* begin() const { return m_ptr; }
	constexpr default_sentinel end() const { return {}; }

	const Type& operator [] (nuint index) const { return m_ptr[index]; }

	constexpr nuint size() const {
		nuint result = 0;
		for(;m_ptr[result] != 0; ++result);
		return result;
	}
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