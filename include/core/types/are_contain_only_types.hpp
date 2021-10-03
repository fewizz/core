#pragma once

#include "indices_of_satisfying_predicate.hpp"
#include "are_contain_type.hpp"

namespace types {

	template<typename... ContainedTypes>
	class are_contain_only_types {

		template<typename... Types>
		struct result {
			template<typename Type>
			static constexpr bool contain = types::are_contain_type<Type>::template for_types_of<Types...>;
		};

	public:

		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		static constexpr bool for_types_of = (result<ContainedTypes...>::template contain<Types> && ...);
	};

}