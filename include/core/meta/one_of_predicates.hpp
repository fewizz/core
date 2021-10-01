#pragma once

#include "../integer.hpp"

template<typename... Predicates>
struct one_of_predicates {

	template<typename... Types>
	static constexpr bool for_types_of =
		(
			(primitive::uint)Predicates::template for_types_of<Types>
			+
			...
		)
		== 1u;

};