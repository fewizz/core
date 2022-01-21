#pragma once

#include "predicate.hpp"

template<typename Type>
concept enum_type = __is_enum(Type);

namespace type {

	struct is_enum : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_enum(Type);

	};

}