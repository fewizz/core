#pragma once

#include "./of_element_type_satisfying_predicate.hpp"
#include "./__type/is_same_as_predicate.hpp"

template<typename Range, typename Element>
concept range_of_element_type_same_as =
	range_of_element_type_satisfying_predicate<
		Range,
		is_same_as<Element>
	>;

template<typename Range, typename Element>
concept range_of_decayed_element_type_same_as =
	range_of_decayed_element_type_satisfying_predicate<
		Range,
		is_same_as<Element>
	>;

// aliases

template<typename Range, typename Element>
concept range_of_value_type = range_of_element_type_same_as<Range, Element>;

template<typename Range, typename Element>
concept range_of = range_of_element_type_same_as<Range, Element>;

template<typename Range, typename Element>
concept range_of_decayed =
	range_of_decayed_element_type_same_as<Range, Element>;