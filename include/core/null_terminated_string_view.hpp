#pragma once

#include <__ranges/view_interface.h>
#include <span>
#include <type_traits>
#include <ranges>

enum class size_is {
	defined, undefined
};

template<size_is Size>
struct null_terminated_string_view;

template<>
struct null_terminated_string_view<size_is::defined> : std::span<const char> {
	using std::span<const char>::span;

	template<std::size_t N>
	constexpr null_terminated_string_view(const char(&arr)[N])
		: std::span<const char>{ arr, N - 1 }
	{}
};

struct null_characted_sentinel {};

constexpr bool operator == (null_characted_sentinel s, const char* ptr) {
	return *ptr == 0;
}

template<>
struct null_terminated_string_view<size_is::undefined>
	: std::ranges::view_interface<null_terminated_string_view<size_is::undefined>>
{
	const char* m_ptr;

	null_terminated_string_view() = default;

	constexpr null_terminated_string_view(const char* p_ptr) : m_ptr{ p_ptr } {};

	constexpr const char* begin() const { return m_ptr; }
	constexpr null_characted_sentinel end() const { return {}; }
};

template<std::size_t N>
null_terminated_string_view(const char(&arr)[N]) -> null_terminated_string_view<size_is::defined>;

null_terminated_string_view(const char*, std::size_t size) -> null_terminated_string_view<size_is::defined>;

template<typename T> requires(std::is_same_v<std::remove_cv_t<T>, const char*>)
null_terminated_string_view(const T&) -> null_terminated_string_view<size_is::undefined>;