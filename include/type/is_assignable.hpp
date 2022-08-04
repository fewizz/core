#pragma once

template<typename Type, typename WhatToAssign>
concept assignable = __is_assignable(Type, WhatToAssign);

#include "predicate.hpp"

namespace type {

	template<typename WhatToAssign>	
	struct is_assignable : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = assignable<Type, WhatToAssign>;

	};

} // type