#pragma once

#include "predicate.hpp"

namespace type {

	struct is_function : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_function(Type);

	};

}

template<typename Type>
inline constexpr bool is_function = type::is_function::for_type<Type>;