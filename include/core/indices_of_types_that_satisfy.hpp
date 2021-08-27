#pragma once

#include "indices.hpp"

template<template<typename T> typename Predicate>
class indices_of_types_that_satisfy {

	template<std::size_t CurrentIndex, std::size_t... Indices>
	struct current_index_and_resulting_indices {

		template<typename... Types>
		struct types {
			using type = indices::of<Indices...>;
		};

		template<typename T, typename... Types>
		requires(Predicate<T>::value)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1,
				Indices..., CurrentIndex
			>::type;
		};

		template<typename T, typename... Types>
		requires(!Predicate<T>::value)
		struct types<T, Types...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1,
				Indices...
			>::type;
		};
	};

public:

	template<typename... Types>
	using of_types
		= typename current_index_and_resulting_indices<0>::template types<Types...>::type;
}; // indices_of_types_that_satisfy

template<template<typename T> typename Predicate>
struct indices_of_types_that_not_satisfy {

	template<typename T>
	struct predicate_negation {
		static constexpr bool value = !Predicate<T>::value;
	};

	template<typename... Types>
	using of_types
		= typename indices_of_types_that_satisfy<predicate_negation>::template of_types<Types...>;
}; // indices_of_types_that_not_satisfy