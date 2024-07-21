#pragma once

#include "../forward.hpp"
#include "./predicate_extensions.hpp"

template<typename Type, typename... Args>
concept list_constructible_from = requires (Args&&... args) {
	Type{ forward<Args>(args)... };
};

template<typename... Args>
struct is_list_constructible_from_t :
	type_predicate_extension<is_list_constructible_from_t<Args...>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return list_constructible_from<Type, Args...>;
	}

};

template<typename Type, typename... Args>
constexpr inline is_list_constructible_from_t<Type, Args...>
	is_list_constructible_from{};