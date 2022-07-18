#pragma once

#include "predicate.hpp"
#include "../../forward.hpp"

template<typename Type, typename... Args>
concept constructible_from = __is_constructible(Type, Args...);

namespace type {

	template<typename... Args>
	struct is_constructible_from : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = __is_constructible(Type, Args...);

	};

} // type