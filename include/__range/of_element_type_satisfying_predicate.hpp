#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../__type/decay.hpp"
#include "../__type/predicate.hpp"

template<typename Range, auto Predicate>
concept range_of_decayed_element_type_satisfying_predicate =
	basic_range<Range> && type_predicate<decltype(Predicate)> &&
	Predicate.template for_type<decay<range_element_type<Range>>>();

template<typename Range, auto Predicate>
concept range_of_element_type_satisfying_predicate =
	basic_range<Range> && type_predicate<decltype(Predicate)> &&
	Predicate.template for_type<range_element_type<Range>>();


template<auto Predicate>
struct is_range_of_element_type_satisfying_predicate_t:
	type_predicate_extension<is_range_of_element_type_satisfying_predicate_t<Predicate>>
{
	template<typename Type>
	constexpr bool for_type() const { return false; }

	template<typename Type>
	requires (
		basic_range<Type> && type_predicate<decltype(Predicate)> &&
		Predicate.template for_type<range_element_type<Type>>()
	)
	constexpr bool for_type() const { return true; }
};

template<auto Predicate>
inline constexpr is_range_of_element_type_satisfying_predicate_t<Predicate> is_range_of_element_type_satisfying_predicate{};


template<auto Predicate>
inline constexpr is_range_of_element_type_satisfying_predicate_t<Predicate> is_range_of{};


#include "../__types/count_of_satisfying_predicate.hpp"  // IWYU pragma: export