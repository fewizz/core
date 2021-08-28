#pragma once

#include "indices.hpp"

namespace indices {

template<template<typename T> typename Predicate>
class of_types_that_satisfy {
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
}; // of_types_that_satisfy

template<template<typename T> typename Predicate>
struct of_types_that_not_satisfy {

	template<typename T>
	struct predicate_negation {
		static constexpr bool value = !Predicate<T>::value;
	};

	template<typename... Types>
	using of_types
		= typename of_types_that_satisfy<predicate_negation>::template of_types<Types...>;
}; // of_types_that_not_satisfy

template<template<auto T> typename Predicate>
class of_values_that_satisfy {

	template<std::size_t CurrentIndex, std::size_t... Indices>
	struct current_index_and_resulting_indices {

		template<auto... Values>
		struct values {
			using type = indices::of<Indices...>;
		};

		template<auto V, auto... Values>
		requires(Predicate<V>::value)
		struct values<V, Values...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1,
				Indices..., CurrentIndex
			>::type;
		};

		template<auto V, auto... Values>
		requires(!Predicate<V>::value)
		struct values<V, Values...> {
			using type = typename current_index_and_resulting_indices<
				CurrentIndex + 1,
				Indices...
			>::type;
		};
	};

public:

	template<typename... Values>
	using of_values
		= typename current_index_and_resulting_indices<0>::template values<Values...>::type;
}; // of_values_that_satisfy

template<template<auto> typename Predicate>
struct of_values_that_not_satisfy {

	template<auto V>
	struct predicate_negation {
		static constexpr bool value = !Predicate<V>::value;
	};

	template<typename... Value>
	using of_values
		= typename of_values_that_satisfy<predicate_negation>::template of_types<Value...>;
}; // of_values_that_not_satisfy

} // indices_of