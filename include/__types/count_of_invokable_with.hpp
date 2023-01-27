#pragma once

#include "./contain_satisfying_predicate.hpp"
#include "../__type/is_invokable_with.hpp"

template<typename... Args>
constexpr inline count_of_satisfying_predicate_t<
	is_invokablew_with<Args...>
> count_of_invokable_with{};