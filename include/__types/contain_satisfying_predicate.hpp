#pragma once

#include "./count_of_satisfying_predicate.hpp"


template<type_predicate auto Predicate>
static constexpr auto contain_satisfying_predicate =
	count_of_satisfying_predicate<Predicate> > 0;

template<type_predicate auto Predicate>
static constexpr auto may_contain_satisfying_predicate =
	count_of_satisfying_predicate<Predicate> >= 0;

template<type_predicate auto Predicate>
static constexpr auto contain_one_satisfying_predicate =
	count_of_satisfying_predicate<Predicate> == 1;

template<type_predicate auto Predicate>
static constexpr auto may_contain_one_satisfying_predicate =
	count_of_satisfying_predicate<Predicate> <= 1;