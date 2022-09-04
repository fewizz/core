#pragma once

#include "./predicate.hpp"

namespace __type {

	template<typename Type, typename BaseType>
	constexpr inline bool is_same_as = __is_same_as(Type, BaseType);

}

template<typename Type, typename BaseType>
concept same_as = __is_same_as(Type, BaseType);

template<typename Type>
struct is_same_as_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename OtherType>
	constexpr bool for_type() const {
		return __is_same_as(Type, OtherType);
	}

};

template<typename Type>
constexpr inline is_same_as_t<Type> is_same_as{};