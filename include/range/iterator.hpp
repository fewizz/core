#pragma once

#include "integer.hpp"
#include "type/is_array.hpp"
#include "type/remove_const.hpp"
#include "type/remove_reference.hpp"

struct range_iterator_t {

	template<typename Type>
	requires is_array<Type>
	constexpr auto operator () (Type& o) const {
		return o;
	}

	template<typename Type>
	requires requires (Type& o) { o.iterator(); }
	constexpr auto operator () (Type& o) const {
		return o.iterator();
	}

};

inline constexpr range_iterator_t range_iterator {};

template<typename Type>
constexpr auto begin(Type& possible_range) {
	return range_iterator(possible_range);
}