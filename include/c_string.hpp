#pragma once

#include "./__type/is_same_as.hpp"
#include "./__type/is_pointer.hpp"
#include "./__type/remove_pointer.hpp"
#include "./__range/extensions.hpp"
#include "./__range/contiguous.hpp"
#include <span.hpp>
#include <move.hpp>

template<typename Type>
struct c_string;

template<typename Type>
concept some_char = same_as_any<
	Type,
	char, wchar_t, char16_t, char32_t, char8_t
>;

// simple one
struct c_string_sentinel_t{};

template<some_char Type>
struct c_string<Type> : range_extensions<c_string<Type>> {
	const Type* ptr_;

	constexpr c_string(const Type* ptr): ptr_{ptr} {}

	static constexpr bool is_borrowed_range = true;

	constexpr const Type*          iterator() const { return ptr_; }
	constexpr c_string_sentinel_t  sentinel() const { return {}; }

	constexpr auto sized() && {
		return as_c_string_convertible(span{
			ptr_,
			this->get_or_compute_size()
		});
	}
};

template<typename Type>
requires (
	!type_is_array<Type> &&
	type_is_pointer<Type> &&
	some_char<remove_const<remove_pointer<Type>>>
)
c_string(Type value) -> c_string<remove_const<remove_pointer<Type>>>;

template<some_char Type>
constexpr bool operator == (c_string_sentinel_t, const Type* ptr) {
	return *ptr == 0;
}

template<some_char Type>
requires same_as_any<Type, char, wchar_t, char16_t, char32_t, char8_t>
constexpr bool operator == (const Type* ptr, c_string_sentinel_t) {
	return *ptr == 0;
}

inline constexpr c_string<char> operator""_c_string (const char* str, nuint) {
	return {str};
}

inline constexpr c_string<char8_t> operator""_c_string (const char8_t* str, nuint) {
	return {str};
}

template<typename Range>
struct c_string_convertible : Range {
	operator c_string<remove_const<decay<range_element_type<Range>>>> () const {
		return { this->elements_ptr() };
	}

};


template<typename Range>
auto as_c_string_convertible(Range&& range) {
	return c_string_convertible{ move(range) };
}

inline constexpr auto
operator""_span (const char* str, nuint len) {
	return as_c_string_convertible(span{str, len});
}

inline constexpr auto
operator""_span (const char8_t* str, nuint len) {
	return as_c_string_convertible(span{str, len});
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"

inline constexpr auto
operator""s (const char* str, nuint len) {
	return as_c_string_convertible(span{str, len});
}

inline constexpr auto
operator""s (const char8_t* str, nuint len) {
	return as_c_string_convertible(span{str, len});
}

#pragma clang diagnostic pop