#pragma once

#include "predicate.hpp"
#include "is_constructible_from.hpp"

template<typename Type>
concept move_constructible = constructible_from<Type, Type&&>;

namespace type {

	struct is_move_constructible : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = move_constructible<Type>;

	};

} // type