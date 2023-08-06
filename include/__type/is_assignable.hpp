#pragma once

#include "predicate.hpp"

template<typename Type, typename WhatToAssign>
concept assignable = __is_assignable(Type, WhatToAssign);

template<typename WhatToAssign>
struct is_assignable_t :
	type_predicate_extension<is_assignable_t<WhatToAssign>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return assignable<Type, WhatToAssign>;
	}

};

template<typename WhatToAssign>
static inline is_assignable_t<WhatToAssign> is_assignable{};