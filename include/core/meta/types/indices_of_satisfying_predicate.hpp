#pragma once

#include "of.hpp"
#include "../values/of.hpp"
#include "../type/predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	class indices_of_satisfying_predicate {

		template<nuint CurrentIndex, nuint... Indices>
		struct current_index_and_resulting_indices {
		
			template<typename... Types>
			struct remaining_types;

			template<typename CurrentType, typename... RemainingTypes>
			requires(Predicate::template for_type<CurrentType>)
			struct remaining_types<CurrentType, RemainingTypes...>
				: type::of<
					typename current_index_and_resulting_indices<CurrentIndex + 1, Indices..., CurrentIndex>
					::template remaining_types<RemainingTypes...>::type
				>{};

			template<typename CurrentType, typename... RemainingTypes>
			requires(!Predicate::template for_type<CurrentType>)
			struct remaining_types<CurrentType, RemainingTypes...>
				: type::of<
					typename current_index_and_resulting_indices<CurrentIndex + 1, Indices...>
					::template remaining_types<RemainingTypes...>::type
				>{};

			template<>
			struct remaining_types<>
				: type::of<indices::of<Indices...>> {};

		};
	
	public:
	
		template<typename... Types>
		using for_types = typename
			current_index_and_resulting_indices<0>
			::template remaining_types<Types...>
			::type;

	};

}