#pragma once

#include "./sized.hpp"
#include "./size_type.hpp"


template<typename Type>
struct range_element_index_type_mark {
	using range_element_index_type = Type;
};

template<typename Type>
concept has_range_element_index_type_mark = requires {
	typename Type::range_element_index_type;
};


template<basic_range Range>
struct range_element_index_type_t {};

template<basic_range Range>
requires has_range_element_index_type_mark<Range>
struct range_element_index_type_t<Range> {
	using type = Range::range_element_index_type;
};

template<sized_range Range>
requires (!has_range_element_index_type_mark<Range>)
struct range_element_index_type_t<Range> {
	using type = range_size_type<Range>;
};

template<basic_range Range>
using range_element_index_type
	= typename range_element_index_type_t<Range>::type;