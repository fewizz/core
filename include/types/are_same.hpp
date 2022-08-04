#pragma once

#include "predicate.hpp"
#include "../type/is_same_as.hpp"

namespace types {

	struct are_same : types::predicate_marker {

		template<typename HeadType, typename... RemainingTypes>
		static constexpr bool for_types
			= (__is_same_as(HeadType, RemainingTypes) && ...);

	};

} // types

template<typename HeadType, typename... RemainingTypes>
inline constexpr bool types_are_same =
	(__is_same_as(HeadType, RemainingTypes) && ...);