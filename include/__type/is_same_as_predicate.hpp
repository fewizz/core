#pragma once

#include "./predicate.hpp"
#include "./is_same_as.hpp"
#include "./decay.hpp"

template<typename Type>
struct is_same_as_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename OtherType>
	constexpr bool for_type() const {
		return __is_same_as(Type, OtherType);
	}

	struct while_decayed_t :
		type_predicate_extension<while_decayed_t>
	{
		template<typename OtherType>
		constexpr bool for_type() const {
			return __is_same_as(decay<Type>, OtherType);
		}
	};

	static constexpr while_decayed_t while_decayed{};

};

template<typename Type>
constexpr inline is_same_as_t<Type> is_same_as{};