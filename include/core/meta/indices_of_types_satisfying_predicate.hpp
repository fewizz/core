#pragma once

#include "indices.hpp"
#include "integer.hpp"
#include "types.hpp"

template<template<typename T> typename Predicate>
class indices_of_types_satisfying_predicate {

	template<uint CurrentIndex, uint... Indices>
	struct current_index_and_resulting_indices {

		template<typename... Types>
		struct types {
			using type = indices::of<Indices...>;
		};

		template<typename T, typename... Types>
		requires(Predicate<T>::value)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices..., CurrentIndex
			>::template types<Types...>::type;
		};

		template<typename T, typename... Types>
		requires(!Predicate<T>::value)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1u,
				Indices...
			>::template types<Types...>::type;
		};
	};

public:

	template<typename... Types>
	using of
		= typename current_index_and_resulting_indices<0u>::template types<Types...>::type;
};