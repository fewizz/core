#pragma once

#include "./predicate.hpp"

namespace type {

	struct is_reference : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_reference(Type);

	};

} // type

template<typename Type>
inline constexpr bool is_reference = __is_reference(Type);