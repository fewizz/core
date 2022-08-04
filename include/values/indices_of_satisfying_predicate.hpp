#pragma once

#include "of.hpp"
#include "../integer.hpp"
#include "../value/predicate.hpp"

namespace values {

	template<value::predicate Predicate>
	class indices_of_satisfying_predicate {

		template<nuint CurrentIndex, nuint... ResultingIndices>
		struct current_index_and_resulting_indices {
		
			template<auto... RemainingValues>
			struct remaining_values;

			template<auto CurrentValue, auto... RemainingValues>
			requires(Predicate::template for_value<CurrentValue>)
			struct remaining_values<CurrentValue, RemainingValues...> :
				type::of<
					typename current_index_and_resulting_indices<
						CurrentIndex + 1,
						ResultingIndices...,
						CurrentIndex
					>::template
					remaining_values<RemainingValues...>::type
				>
			{};

			template<auto CurrentValue, auto... RemainingValues>
			requires(!Predicate::template for_value<CurrentValue>)
			struct remaining_values<CurrentValue, RemainingValues...> :
				type::of<
					typename current_index_and_resulting_indices<
						CurrentIndex + 1,
						ResultingIndices...
					>::template
					remaining_values<RemainingValues...>::type
				>
			{};

			template<>
			struct remaining_values<> :
				type::of<indices::of<ResultingIndices...>>
			{};

		};

	public:

		template<auto... Values>
		using for_values = typename
			current_index_and_resulting_indices<0>
			::template remaining_values<Values...>
			::type;
	
	};

}