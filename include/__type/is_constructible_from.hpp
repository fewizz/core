#pragma once

#include "./predicate.hpp"

template<typename Type, typename... Args>
concept constructible_from = __is_constructible(Type, Args...);

template<typename... Args>
struct is_constructible_from_t :
	type_predicate_extension<is_constructible_from_t<Args...>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return constructible_from<Type, Args...>;
	}

};

template<typename... Args>
constexpr is_constructible_from_t<Args...> is_constructible_from{};