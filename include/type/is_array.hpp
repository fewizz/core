#pragma once

#include "./predicate.hpp"

namespace type {

	struct is_array : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_array(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_array = __is_array(Type);