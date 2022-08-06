#pragma once

#include "predicate.hpp"

namespace type {

	struct is_enum : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_enum(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_enum = __is_enum(Type);