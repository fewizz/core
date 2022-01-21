#pragma once

#include "../integer.hpp"
#include "of.hpp"
#include "../value/predicate.hpp"

namespace values {

	template<value::predicate Predicate>
	class indices_of_satisfying_predicate {
	
		template<nuint CurrentIndex, nuint... Indices>
		struct current_index_and_resulting_indices {
		
			template<auto... Values>
			struct resulting {
				using indices_type = indices::of<Indices...>;
			};
	
			template<auto Value, auto... Values>
			requires(Predicate::template for_value_of<Value>)
			struct resulting<Value, Values...> {
				using indices_type = typename current_index_and_resulting_indices<
					CurrentIndex + 1,
					Indices..., CurrentIndex
				>::template resulting<Values...>::indices_type;
			};
	
			template<auto Value, auto... Values>
			requires(!Predicate::template for_value_of<Value>)
			struct resulting<Value, Values...> {
				using indices_type = typename current_index_and_resulting_indices<
					CurrentIndex + 1,
					Indices...
				>::template resulting<Values...>::indices_type;
			};
		};
	
	public:
	
		template<auto... Values>
		using for_values_of = typename
			current_index_and_resulting_indices<0>
			::template resulting<Values...>
			::indices_type;
	};

}