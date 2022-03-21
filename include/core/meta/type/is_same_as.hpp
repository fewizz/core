#pragma once

#include "predicate.hpp"

namespace type {

	template<typename BaseType>
	struct is_same_as : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_same_as(Type, BaseType);

	};

} // type