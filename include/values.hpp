#pragma once

#include "./__values/at_index.hpp"  // IWYU pragma: export
#include "./__values/contain_equal_to.hpp"  // IWYU pragma: export
#include "./__values/erase_at_index.hpp"  // IWYU pragma: export
#include "./__values/first.hpp"  // IWYU pragma: export
#include "./__values/indices_of_equal_to.hpp"  // IWYU pragma: export
#include "./__values/indices_of_satisfying_predicate.hpp"  // IWYU pragma: export
#include "./__values/permutations_parity.hpp"  // IWYU pragma: export
#include "./__values/permutations.hpp"  // IWYU pragma: export
#include "./__values/push_front.hpp"  // IWYU pragma: export

template<auto... Values>
struct values {

	template<nuint Index>
	static constexpr auto at_index
		= __values::at_index<Index>::template for_values<Values...>;

	

};