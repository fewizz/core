#pragma once

#include "./basic.hpp"
#include "./element_type.hpp"
#include "./__type/decay.hpp"

template<typename Range, auto Predicate>
concept range_of_value_type_satisfying_predicate =
	basic_range<Range> && type_predicate<decltype(Predicate)> &&
	Predicate.template for_type<decay<range_element_type<Range>>>();

#include "./__types/count_of_satisfying_predicate.hpp"
#include "./__types/contain_satisfying_predicate.hpp"