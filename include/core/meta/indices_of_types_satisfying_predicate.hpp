#pragma once

#include "../integer.hpp"
#include "types.hpp"
#include "values.hpp"

template<typename Predicate>
class indices_of_types_satisfying_predicate {

	template<uint CurrentIndex, uint... Indices>
	struct current_index_and_resulting_indices {

		template<typename... Types>
		struct types {
			using type = indices_of<Indices...>;
		};

		template<typename T, typename... Types>
		requires(Predicate::template for_type_of<T>)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices..., CurrentIndex
			>::template types<Types...>::type;
		};

		template<typename T, typename... Types>
		requires(!Predicate::template for_type_of<T>)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices...
			>::template types<Types...>::type;
		};
	};

public:

	template<typename... Types>
	using for_types_of = typename
		current_index_and_resulting_indices<0u>
		::template types<Types...>
		::type;
};