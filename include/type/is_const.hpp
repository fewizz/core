#pragma once

#include "predicate.hpp"

namespace type {

	struct is_const : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_const(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_const = __is_const(Type);