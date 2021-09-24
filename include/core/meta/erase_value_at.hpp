#pragma once

#include "integer.hpp"

#include "values.hpp"

template <uint Index>
class erase_value_at {

	template<auto... LeftValues>
	struct left_values {
		template<uint CurrentIndex>
		struct current_value_index {

			template<auto CurrentValue, auto... RightValues>
			struct right_values {
				using type = typename
					left_values<LeftValues..., CurrentValue>::
					template current_value_index<CurrentIndex + 1u>::
					template right_values<RightValues...>
					::type;
			};
		};

		template<>
		struct current_value_index<Index> {
			template<auto CurrentValue, auto... RightValues>
			struct right_values {
				using type = values_of<LeftValues..., RightValues...>;
			};
		};
	};

public:
	
	template<auto... Values>
	using of = typename left_values<>::template current_value_index<0u>::template right_values<Values...>::type;
};