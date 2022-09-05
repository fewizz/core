#pragma once

#include "./__values/at_index.hpp"
#include "./__values/contain_equal_to.hpp"
#include "./__values/erase_at_index.hpp"
#include "./__values/first.hpp"
#include "./__values/indices_of_equal_to.hpp"
#include "./__values/indices_of_satisfying_predicate.hpp"
#include "./__values/permutations_parity.hpp"
#include "./__values/permutations.hpp"
#include "./__values/push_front.hpp"

template<auto... Values>
struct values {

	template<nuint Index>
	static constexpr auto at_index
		= __values::at_index<Index>::template for_values<Values...>;

	

};