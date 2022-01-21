#pragma once

#include "predicate.hpp"
#include "../type/modifier.hpp"

namespace types {

	template<types::predicate Predicate, type::modifier Modifier>
	struct modified_predicate : types::predicate_marker {

		template<typename... Types>
		static constexpr bool for_types =
			Predicate::template for_types<
				typename Modifier::template for_type<Types>...
			>;

		template<typename... Types>
		using indices_of_affected_types =
			typename Predicate::template indices_of_affected_types<
				typename Modifier::template for_type<Types>...
			>;

	};

}