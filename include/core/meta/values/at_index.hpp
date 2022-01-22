#pragma once

#include "../value/of.hpp"
#include "../../integer.hpp"

namespace values {

	template<nuint Index>
	class at_index {

		template<nuint CurrentIndex, auto... RemainingValues>
		struct current_index_and_remaining_values;

		template<auto CurrentValue, auto... RemainingValues>
		struct current_index_and_remaining_values<Index, CurrentValue, RemainingValues...>
			: value::of<CurrentValue> {};

		template<nuint CurrentIndex, auto CurrentValue, auto... RemainingValues>
		requires(sizeof...(RemainingValues) > 0)
		struct current_index_and_remaining_values<CurrentIndex, CurrentValue, RemainingValues...>
			: value::of<
				current_index_and_remaining_values<
					CurrentIndex + 1,
					RemainingValues...
				>::value
			>{};

	public:

		template<auto...Values>
		static constexpr auto for_values = current_index_and_remaining_values<0, Values...>::value;
	};

}