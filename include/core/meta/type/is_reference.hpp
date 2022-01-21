#pragma once

#include "predicate.hpp"

namespace type {

	struct is_reference : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = false;

		template<typename Type>
		static constexpr bool for_type<Type&> = true;

		template<typename Type>
		static constexpr bool for_type<Type&&> = true;

	};

}