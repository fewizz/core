#pragma once

#include "../integer.hpp"

namespace __type {

	template<typename Type>
	struct remove_extent_t          { using type = Type; };

	template<typename Type, nuint N>
	struct remove_extent_t<Type[N]> { using type = Type; };

	template<typename Type>
	using remove_extent = typename remove_extent_t<Type>::type;

}