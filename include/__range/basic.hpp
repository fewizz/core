#pragma once

#include "../__type/is_array.hpp"
#include "../__type/is_reference.hpp"
#include "../__type/remove_const.hpp"
#include "../__type/is_same_as.hpp"
#include "../__type/remove_reference.hpp"
#include "../__iterator/basic.hpp"
#include "../__iterator/sentinel.hpp"

template<typename RangeType>
concept basic_range =
	requires(RangeType r) {
		{ r.iterator() } -> basic_iterator;
		{ r.iterator() } -> __type::not_a_reference;
		{ r.sentinel() } -> __type::not_a_reference;
		{ r.sentinel() } -> sentinel_for<decltype(r.iterator())>;
	} ||
	type_is_array<remove_reference<RangeType>>;