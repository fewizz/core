#pragma once

#include "predicate.hpp"
#include "erase_at_indices.hpp"
#include "../value/of.hpp"

namespace types {

	template<types::predicate... Predicates>
	class are_exclusively_satsify_predicates : public types::predicate_marker {

		template<typename... RemainingTypes>
		struct remaining_types {

			template<types::predicate... RemainingPredicates>
			struct remaining_predicates;

			template<types::predicate HeadPredicate, types::predicate... RemainingPredicates>
			requires(! HeadPredicate::template for_types<RemainingTypes...>)
			struct remaining_predicates<HeadPredicate, RemainingPredicates...> : value::of<false> {};

			template<types::predicate HeadPredicate, types::predicate... RemainingPredicates>
			requires(HeadPredicate::template for_types<RemainingTypes...> && sizeof...(RemainingPredicates) > 0)
			struct remaining_predicates<HeadPredicate, RemainingPredicates...>
				: value::of<
					HeadPredicate::
					template indices_of_affected_types<RemainingTypes...>::
					template pass_for_type_directly<types::erase_at_indices>::
					template for_types<RemainingTypes...>::
					template pass_for_type_directly<remaining_types>::
					template remaining_predicates<RemainingPredicates...>::
					value
				>{};

			template<types::predicate HeadPredicate>
			requires(HeadPredicate::template for_types<RemainingTypes...>)
			struct remaining_predicates<HeadPredicate>
				: value::of<
					HeadPredicate::
					template indices_of_affected_types<RemainingTypes...>::
					template pass_for_type_directly<types::erase_at_indices>::
					template for_types<RemainingTypes...>::
					is_empty
				>{};

		};

	public:

		template<typename... Types>
		static constexpr bool for_types =
			remaining_types<Types...>::template
			remaining_predicates<Predicates...>::
			value
		;

	};

}