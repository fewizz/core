#pragma once

#include "./predicate.hpp"
#include "../__range/of_element_type_same_as.hpp"

template<typename Type>
struct is_range_of_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename Range>
	constexpr bool for_type() const {
		return range_of<Range, Type>;
	}

};

template<typename Type>
struct is_range_of_decayed_t : type_predicate_extension<is_same_as_t<Type>> {

	template<typename Range>
	constexpr bool for_type() const {
		return range_of_decayed<Range, Type>;
	}

};

template<typename Type>
constexpr inline is_range_of_t<Type> is_range_of{};

template<typename Type>
constexpr inline is_range_of_decayed_t<Type> is_range_of_decayed{};