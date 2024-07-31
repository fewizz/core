#pragma once

#include "./predicate.hpp"

struct are_same : types_predicate_mark {

	template<typename HeadType, typename... RemainingTypes>
	constexpr bool for_types() const {
		return (__is_same_as(HeadType, RemainingTypes) && ...);
	}

};

template<typename HeadType, typename... RemainingTypes>
inline constexpr bool types_are_same =
	(__is_same_as(HeadType, RemainingTypes) && ...);