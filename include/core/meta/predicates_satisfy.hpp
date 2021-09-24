#pragma once

#include "value.hpp"

template<template<typename...> typename... Predicates>
struct predicates_satisfy;

template<template<typename...> typename Predicate>
struct predicates_satisfy<Predicate> {
	template<typename... Types>
	static constexpr bool for_types_of = Predicate<Types...>::value;
};

template<template<typename...> typename PredicateHead, template<typename...> typename... PredicateTail>
struct predicates_satisfy<PredicateHead, PredicateTail...> {

	template<typename... Types>
	static constexpr bool for_types_of =
		PredicateHead<Types...>::value
		&& predicates_satisfy<PredicateTail...>::template of<Types...>::value
	;
};
