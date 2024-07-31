#pragma once

#include "./without_extensions.hpp"
#include "../range.hpp"
#include "../__type/is_constructible_from.hpp"

template<typename Type, nuint Size>
struct array :
	array_without_extensions<Type, Size>,
	range_extensions<array<Type, Size>>
{
private:
	using base_type = array_without_extensions<Type, Size>;
public:

	template<typename... Args>
	requires constructible_from<Type[Size], Args&&...>
	constexpr array(Args&&... args) :
		base_type{ forward<Args>(args)... }
	{}

};

template<typename... Types>
requires(sizeof...(Types) == 1 || types_are_same<Types...>)
array(Types&&...)
	-> array<
		remove_reference<first_type<Types...>>,
		sizeof...(Types)
	>;