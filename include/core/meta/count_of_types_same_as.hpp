#pragma once

#include "../integer.hpp"
#include "indices_of_types_same_as.hpp"

template<typename Type>
struct count_of_types_same_as {

	template<typename... Types>
	static constexpr uint for_types_of =
		indices_of_types_same_as<Type>
		::template for_types_of<Types...>
		::size;

	template<typename... Types>
	using for_types_of_t = value_of<for_types_of<Types...>>;
};