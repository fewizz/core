#pragma once

#include "predicate.hpp"

namespace type {

	struct is_function : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_function(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_function = __is_function(Type);