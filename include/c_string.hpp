#pragma once

#include "./__iterator_and_sentinel/distance.hpp"
#include "./__type/remove_pointer.hpp"
#include "./__type/remove_const.hpp"
#include "./__type/is_pointer.hpp"
#include "./__type/is_const.hpp"

enum class c_string_type {
	known_size, unknown_size
};

template<c_string_type Sized, typename Type = char>
/* *sentinel() == '\0' */
struct c_string;

struct c_string_sentinel{};

template<typename Type>
struct c_string<c_string_type::unknown_size, Type> {
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
			ptr_, __iterator_and_sentinel::distance(iterator(), sentinel())
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

using c_string_of_unknown_size = c_string<c_string_type::unknown_size>;
using c_string_of_known_size   = c_string<c_string_type::known_size>;

template<typename Type, nuint Size>
c_string(Type(&)[Size]) -> c_string<
	c_string_type::known_size,
	Type
>;

template<typename Type>
c_string(const Type*, nuint size) -> c_string<
	c_string_type::known_size,
	Type
>;

template<typename Type>
requires
	__type::is_pointer<Type> &&
	__type::is_const<__type::remove_pointer<Type>>
c_string(Type) -> c_string<
	c_string_type::unknown_size,
	__type::remove_const<__type::remove_pointer<Type>>
>;