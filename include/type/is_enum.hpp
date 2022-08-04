#pragma once

#include "predicate.hpp"

namespace type {

	struct is_enum : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_enum(Type);

	};

} // type