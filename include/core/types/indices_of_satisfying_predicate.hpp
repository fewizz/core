#pragma once

#include "../integer.hpp"
#include "../values/of.hpp"
#include "of.hpp"
#include "../type/predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	class indices_of_satisfying_predicate {
	
		template<nuint CurrentIndex, nuint... Indices>
		struct current_index_and_resulting_indices {
		
			template<typename... Types>
			struct resulting {
				using indices_type = indices::of<Indices...>;
			};
	
			template<typename T, typename... Types>
			requires(Predicate::template for_type_of<T>)
			struct resulting<T, Types...> {
				using indices_type = typename current_index_and_resulting_indices<
					CurrentIndex + 1u,
					Indices..., CurrentIndex
				>::template resulting<Types...>::indices_type;
			};
	
			template<typename T, typename... Types>
			requires(!Predicate::template for_type_of<T>)
			struct resulting<T, Types...> {
				using indices_type = typename current_index_and_resulting_indices<
					CurrentIndex + 1u,
					Indices...
				>::template resulting<Types...>::indices_type;
			};
		};
	
	public:
	
		template<typename... Types>
		using for_types_of = typename
			current_index_and_resulting_indices<0u>
			::template resulting<Types...>
			::indices_type;
	};

}