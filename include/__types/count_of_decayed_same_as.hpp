#pragma once

#include "./count_of_satisfying_predicate.hpp"
#include "../__type/is_same_as.hpp"

template<typename Type>
constexpr inline count_of_satisfying_predicate_t<
	is_same_as<Type>.while_decayed
> count_of_decayed_same_as{};