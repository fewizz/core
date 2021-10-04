#pragma once

#include "erase_types.hpp"

namespace types {

	template<typename... ContainedTypes>
	struct are_contain_only_types {

		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		static constexpr bool for_types_of = types::erase_types<ContainedTypes...>::template for_types_of<Types...>::is_empty;
	};

}