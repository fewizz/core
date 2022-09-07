#pragma once

#include "./predicate.hpp"
#include "./is_same_as.hpp"
#include "./decay.hpp"

template<typename Type>
struct is_same_as_while_decayed_t :
	type_predicate_extension<is_same_as_while_decayed_t<Type>>
{
	template<typename ThisType>
	constexpr bool for_type() const {
		return __is_same_as(decay<ThisType>, Type);
	}
};

template<typename Type>
struct is_same_as_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename ThisType>
	constexpr bool for_type() const {
		return __is_same_as(ThisType, Type);
	}

	static constexpr is_same_as_while_decayed_t<Type> while_decayed{};

};

template<typename Type>
constexpr inline is_same_as_t<Type> is_same_as{};