#pragma once

#include "erase_types.hpp"
#include "predicate.hpp"

namespace types {

	template<typename... ContainedTypes>
	struct are_contain_only_types : types::predicate_marker {

		template<typename... Types>
		static constexpr bool for_types_of = types::erase_types<ContainedTypes...>::template for_types_of<Types...>::is_empty;
	};

}