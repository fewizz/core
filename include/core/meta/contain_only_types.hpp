#pragma once

#include "indices_of_types_satisfying_predicate.hpp"
#include "contain_type.hpp"

template<typename... ContainedTypes>
class contain_only_types {

	template<typename... Types>
	struct types {
		template<typename Type>
		static constexpr bool contain = contain_type<Type>::template for_types_of<Types...>;
	};

public:

	template<typename... Types>
	static constexpr bool for_types_of = (types<ContainedTypes...>::template contain<Types> && ...);
};