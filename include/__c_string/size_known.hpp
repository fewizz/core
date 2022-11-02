#pragma once

#include "../span.hpp"
#include "./declaration.hpp"

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