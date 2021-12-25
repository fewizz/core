#pragma once

#include "predicate.hpp"
#include "erase_at_indices.hpp"

namespace types {

	template<types::predicate... Predicates>
	class are_exclusively_satsify_predicates {

		template<typename... Types>
		struct remaining_types {

			template<types::predicate... RemainingPredicates>
			struct remaining_predicates {
				template<typename...>
				static constexpr bool result = sizeof...(Types) == 0;
			};

			template<types::predicate HeadPredicate, types::predicate... TailPredicates>
			struct remaining_predicates<HeadPredicate, TailPredicates...> {
		 
				static constexpr bool satisfy = HeadPredicate::template for_types_of<Types...>;
				
				using indices_of_affected_types = typename HeadPredicate::template indices_of_affected_types_of<Types...>;

				using after_erasing =
					typename indices_of_affected_types::
					template pass_for_type_directly<types::erase_at_indices>::
					template for_types_of<Types...>;

				template<typename...>
				static constexpr bool result = false;

				template<typename...Ts>
				requires(satisfy)
				static constexpr bool result<Ts...> =
					after_erasing::template pass_for_type_directly<remaining_types>
					::template remaining_predicates<TailPredicates...>::template result<>;
			};
		};

	public:

		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		static constexpr bool for_types_of =
			remaining_types<Types...>
			::template remaining_predicates<Predicates...>
			::template result<>
		;

	};

}