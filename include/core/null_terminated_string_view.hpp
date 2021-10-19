#pragma once

#include "integer.hpp"
#include "type/remove_const.hpp"
#include "types/are_same.hpp"

enum class size_is {
	defined, undefined
};

template<size_is Size>
struct null_terminated_string_view;

template<>
struct null_terminated_string_view<size_is::defined> {

	using value_type = char;
	
	const char* m_ptr;
	uint m_size;

	constexpr null_terminated_string_view(const char* arr, uint size)
		: m_ptr{ arr }, m_size{ size }
	{}

	template<uint N>
	constexpr null_terminated_string_view(const char(&arr)[N])
		: m_ptr{ arr }, m_size{ N - 1 }
	{}

	constexpr const char* begin() const {
		return m_ptr;
	}

	constexpr const char* end() const {
		return m_ptr + m_size; // TODO
	}

	constexpr uint size() const {
		return m_size;
	}
};

struct null_characted_sentinel {};

constexpr bool operator == (null_characted_sentinel s, const char* ptr) {
	return *ptr == 0;
}

template<>
struct null_terminated_string_view<size_is::undefined> {

	using value_type = char;

	const char* m_ptr = nullptr;

	null_terminated_string_view() = default;

	constexpr null_terminated_string_view(const char* p_ptr)
		: m_ptr{ p_ptr }
	{}

	constexpr const char* begin() const { return m_ptr; }
	constexpr null_characted_sentinel end() const { return {}; }
};

template<uint N>
null_terminated_string_view(const char(&arr)[N])
	-> null_terminated_string_view<size_is::defined>;

null_terminated_string_view(const char*, uint size)
	-> null_terminated_string_view<size_is::defined>;

template<typename T> requires(types::are_same::for_types_of<type::remove_const::for_type_of<T>, const char*>)
null_terminated_string_view(const T&) -> null_terminated_string_view<size_is::undefined>;