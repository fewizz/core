#pragma once

#include "predicate.hpp"

namespace types {

	struct are_same : types::predicate_marker {

		template<typename... Types>
		requires(sizeof...(Types) >= 2)
		static constexpr bool for_types = false;

		template<typename Type>
		static constexpr bool for_types<Type, Type> = true;

		template<typename HeadType, typename... RemainingTypes>
		static constexpr bool for_types<HeadType, HeadType, RemainingTypes...>
			= for_types<HeadType, RemainingTypes...>;

	};

}

template<typename... Types>
inline constexpr bool types_are_same = types::are_same::for_types<Types...>;