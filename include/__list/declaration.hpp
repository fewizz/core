#pragma once

#include "../__range/basic.hpp"
#include "../__range/borrowed.hpp"
#include "../__storage/range.hpp"

template<basic_range StorageRange>
requires (
	!borrowed_range<StorageRange> &&
	type_is_lvalue_reference<range_element_type<StorageRange>>
)
class list;