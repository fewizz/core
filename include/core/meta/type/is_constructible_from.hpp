#pragma once

#include "predicate.hpp"
#include "../../forward.hpp"

namespace type {

	template<typename Type, typename... Args>
	concept constructible_from = requires(Args&&... args) {
		Type{ forward<Args>(args)... };
	};

	template<typename... Types>
	struct is_constructible_from : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = type::constructible_from<Type, Types...>;

	};
}