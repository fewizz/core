#pragma once

#include "../type/modifier.hpp"
#include "predicate.hpp"

namespace types {

	template<types::predicate Predicate, type::modifier Modifier>
	struct modified_predicate : types::predicate_marker {

		template<typename... Types>
		static constexpr bool for_types_of =
			Predicate::template for_types_of<
				typename Modifier::template for_type<Types>...
			>;

		template<typename... Types>
		using indices_of_affected_types_of =
			typename Predicate::template indices_of_affected_types_of<
				typename Modifier::template for_type<Types>...
			>;

	};

}