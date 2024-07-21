#pragma once

namespace __type {

	template<typename Type, typename OtherType>
	constexpr inline bool is_same_as = __is_same_as(Type, OtherType);

}

template<typename Type, typename OtherType>
concept same_as = __is_same_as(Type, OtherType);

template<typename Type, typename... OtherTypes>
concept same_as_any = (same_as<Type, OtherTypes> || ...);

template<typename Type, typename OtherType>
concept not_same_as = !same_as<Type, OtherType>;


#include "./predicate_extensions.hpp"


template<typename Type>
struct is_same_as_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename ThisType>
	constexpr bool for_type() const {
		return __is_same_as(ThisType, Type);
	}

};

template<typename Type>
constexpr inline is_same_as_t<Type> is_same_as{};