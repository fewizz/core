#pragma once

#include "./predicate.hpp"

namespace type {

	struct is_pointer : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_pointer(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_pointer = __is_pointer(Type);