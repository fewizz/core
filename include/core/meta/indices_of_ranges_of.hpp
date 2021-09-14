#pragma once

#include "indices_of_satisfying.hpp"
#include "is_range_of.hpp"

template<typename Type>
struct indices_of_ranges_of {

	template<typename... Types>
	using of_types = indices_of_satisfying<
		is_range<Types...>::template of
	>;

};