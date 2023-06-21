#pragma once

#include "../span.hpp"
#include "./declaration.hpp"

template<typename Type, typename SizeType>
struct c_string<c_string_type::known_size, Type, SizeType> :
	span<const Type, SizeType>
{
private:
	using base_type = span<const Type, SizeType>;
public:

	c_string() = default;

	template<contiguous_range Range>
	c_string(Range&& range) : base_type{ forward<Range>(range) } {}

	template<nuint Size>
	constexpr c_string(const Type (&array)[Size]) :
		base_type{ array, Size - 1}
	{}

	constexpr c_string(const Type* ptr, SizeType size) :
		base_type{ ptr, size }
	{}

	using base_type::operator = ;
};

template<typename Type, nuint Size>
c_string(const Type(&)[Size]) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;

template<typename Type>
c_string(const Type*, nuint size) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;