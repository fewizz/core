#pragma once

#include "./__iterator_and_sentinel/get_or_compute_distance.hpp"
#include "./range.hpp"
#include "type.hpp"

enum class c_string_type {
	known_size, unknown_size
};

/* it's *sentinel() == '\0' */
template<c_string_type Sized, typename Type = char>
struct c_string;

using c_string_of_unknown_size = c_string<c_string_type::unknown_size>;
using c_string_of_known_size   = c_string<c_string_type::known_size>;

template<typename Type>
concept any_c_string =
	same_as<Type, c_string_of_known_size> ||
	same_as<Type, c_string_of_unknown_size>;

struct c_string_sentinel{};

template<typename Type>
struct c_string<c_string_type::unknown_size, Type> :
	range_extensions<c_string<c_string_type::unknown_size, Type>>
{
private:
	const Type* ptr_;
public:

	constexpr c_string(const Type* ptr) : ptr_{ ptr } {}

	constexpr const Type*       iterator() const { return ptr_; }
	constexpr c_string_sentinel sentinel() const { return {}; }

	constexpr const Type* elements_ptr() const { return ptr_; }

	constexpr const Type& operator [] (nuint index) const {
		return ptr_[index];
	}

	constexpr c_string<c_string_type::known_size, Type> sized() const {
		return {
			ptr_,
			__iterator_and_sentinel::get_or_compute_distance(
				iterator(),
				sentinel()
			)
		};
	}

};

constexpr bool operator == (c_string_sentinel, const auto* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const auto* ptr, c_string_sentinel) {
	return *ptr == 0;
}

#include "./span.hpp"

template<typename Type>
struct c_string<c_string_type::known_size, Type> : span<const Type, nuint> {
private:
	using base_type = span<const Type, nuint>;
public:

	using base_type::base_type;

	template<nuint Size>
	constexpr c_string(const Type (&array)[Size]) :
		base_type{ array, Size - 1}
	{}

	using base_type::operator = ;
};

template<typename Type, nuint Size>
c_string(Type(&)[Size]) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;

template<typename Type>
c_string(Type*, nuint size) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;

template<typename Type>
requires
	type<Type>::is_pointer
c_string(Type) -> c_string<
	c_string_type::unknown_size,
	remove_const<remove_pointer<Type>>
>;