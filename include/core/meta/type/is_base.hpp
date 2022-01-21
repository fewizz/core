#pragma once

#include "modified_predicate.hpp"

namespace type {

	template<typename BaseType>
	struct is_base : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_base_of(BaseType, Type);

	};

}