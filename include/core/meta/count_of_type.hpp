#pragma once

#include "../integer.hpp"
#include "indices_of_type.hpp"

template<typename Type>
struct count_of_type {

	template<typename... Types>
	static constexpr uint for_types_of =
		indices_of_type<Type>
		::template for_types_of<Types...>
		::size;

	template<uint N>
	struct less_than_equals {
		template<typename... Types>
		static constexpr bool for_types_of = count_of_type::for_types_of<Types...> <= N;
	};

	template<uint N>
	struct equals {
		template<typename... Types>
		static constexpr bool for_types_of = count_of_type::for_types_of<Types...> == N;
	};
};