#pragma once

#include "./count_of_satisfying_predicate.hpp"
#include "../__type/is_range_of.hpp"

template<typename Type>
constexpr inline count_of_satisfying_predicate_t<
	is_range_of<Type>
> count_of_range_of{};

template<typename Type>
constexpr inline count_of_satisfying_predicate_t<
	is_range_of_decayed<Type>
> count_of_range_of_decayed{};