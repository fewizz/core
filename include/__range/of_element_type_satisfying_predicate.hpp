#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "../__type/predicate.hpp"

template<typename Range, auto Predicate>
concept range_of =
	basic_range<Range> && type_predicate<decltype(Predicate)> &&
	Predicate.template for_type<range_element_type<Range>>();


template<type_predicate auto Predicate>
struct is_range_of_t:
	type_predicate_extensions<is_range_of_t<Predicate>>
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


template<type_predicate auto Predicate>
inline constexpr is_range_of_t<Predicate> is_range_of{};