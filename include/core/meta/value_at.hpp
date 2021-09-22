#pragma once

#include "../integer.hpp"

template<uint Index>
class value_at {
	template<uint CurrentIndex, auto... Values>
	struct current_index_and_values_left;

	template<auto Value, auto... Values>
	struct current_index_and_values_left<Index, Value, Values...> {
		static constexpr auto value = Value;
	};

	template<uint CurrentIndex, auto Value, auto... Values>
	struct current_index_and_values_left<CurrentIndex, Value, Values...> {
		static constexpr auto value
			= current_index_and_values_left<
				CurrentIndex + 1u,
				Values...
			>::value;
	};

public:
	template<auto...Values>
	static constexpr auto of = current_index_and_values_left<0u, Values...>::value;
};