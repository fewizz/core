#pragma once

#include "value.hpp"

template<template<typename...> typename... Predicates>
struct are_predicates_satisfied_for_types;

template<template<typename...> typename Predicate>
struct are_predicates_satisfied_for_types<Predicate> {

	template<typename... Types>
	static constexpr bool of = Predicate<Types...>::value;
};

template<template<typename...> typename PredicateHead, template<typename...> typename... PredicateTail>
struct are_predicates_satisfied_for_types<PredicateHead, PredicateTail...> {

	template<typename... Types>
	static constexpr bool of =
		PredicateHead<Types...>::value
		&& are_predicates_satisfied_for_types<PredicateTail...>::template of<Types...>::value
	;
};
