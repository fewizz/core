#pragma once

#include "./__type/is_same_as.hpp"
#include "./__type/is_pointer.hpp"
#include "./__type/remove_pointer.hpp"
#include "./__range/extensions.hpp"
#include "./__range/borrowed.hpp"
#include "./__iterator_and_sentinel/get_or_compute_distance.hpp"
#include "./span.hpp"

template<typename Type>
struct c_string;

template<typename Range>
struct c_string_convertible : Range {
	using char_type = decay<range_element_type<Range>>;

	operator c_string<char_type> () const {
		return c_string<char_type>{ this->iterator() };
	}
};

template<typename Type>
struct c_string_view :
	span<const Type>
{
	explicit constexpr c_string_view(const Type* ptr, nuint len)
		: span<const Type>{ptr, len} {}

	template<typename NewType>
	c_string_view<NewType> casted() {
		return { (NewType*) this->ptr_, this->size_ };
	}

	operator c_string<Type> () const {
		return { this->ptr_ };
	}
};

template<typename Type>
concept some_char = same_as_any<
	Type,
	char, wchar_t, char16_t, char32_t, char8_t
>;

// simple one
struct c_string_sentinel_t{};

template<some_char Type>
struct c_string<Type> :
	range_extensions<c_string<Type>>,
	borrowed_range_mark<true>
 {
	const Type* ptr_;

	explicit constexpr c_string(const Type* ptr): ptr_{ptr} {}

	constexpr const Type*          iterator() const { return ptr_; }
	constexpr c_string_sentinel_t  sentinel() const { return {}; }

	template<typename NewType>
	c_string<NewType> casted() {
		return { (NewType*) ptr_ };
	}

	constexpr auto sized() && {
		return c_string_convertible{ span{
			ptr_,
			__iterator_and_sentinel::get_or_compute_distance(
				iterator(), sentinel()
			)
		}};
	}

	constexpr auto sized_view() && {
		return c_string_view {
			ptr_,
			__iterator_and_sentinel::get_or_compute_distance(
				iterator(), sentinel()
			)
		};
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuser-defined-literals"

inline constexpr auto
operator""s (const char* str, nuint) {
	return c_string<char>{str};
}

inline constexpr auto
operator""s (const char8_t* str, nuint) {
	return c_string<char8_t>{str};
}

inline constexpr auto
operator""sv (const char* str, nuint len) {
	return c_string_view{str, len};
}

inline constexpr auto
operator""sv (const char8_t* str, nuint len) {
	return c_string_view{str, len};
}

#pragma clang diagnostic pop