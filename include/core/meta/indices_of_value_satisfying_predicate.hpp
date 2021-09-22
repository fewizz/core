#pragma once

#include "indices.hpp"
#include "integer.hpp"

template<template<auto T> typename Predicate>
class indices_of_values_satisfying_predicate {

	template<uint CurrentIndex, uint... Indices>
	struct current_index_and_resulting_indices {

		template<auto... Values>
		struct values {
			using type = indices::of<Indices...>;
		};

		template<auto V, auto... Values>
		requires(Predicate<V>::value)
		struct values<V, Values...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices..., CurrentIndex
			>::template values<Values...>::type;
		};

		template<auto V, auto... Values>
		requires(!Predicate<V>::value)
		struct values<V, Values...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices...
			>::template values<Values...>::type;
		};
	};

public:

	template<auto... Values>
	using of_values
		= typename current_index_and_resulting_indices<0u>::template values<Values...>::type;
};