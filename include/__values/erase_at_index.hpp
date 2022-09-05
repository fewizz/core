#pragma once

#include "../__values/of.hpp"
#include "../integer.hpp"

namespace __values {

	template <nuint Index>
	class erase_at_index {

		template<auto... ValuesBefore>
		struct values_before {

			template<nuint CurrentIndex>
			struct current_value_index {

				template<auto CurrentValue, auto... ValuesAfter>
				struct current_value_and_values_after {
					using type =
						typename values_before<
							ValuesBefore...,
							CurrentValue
						>::template
						current_value_index<CurrentIndex + 1>::template
						current_value_and_values_after<ValuesAfter...>::type;
				};

			};
	
			template<>
			struct current_value_index<Index> {

				template<auto CurrentValue, auto... ValuesAfter>
				struct current_value_and_values_after {
					using type = __values::of<ValuesBefore..., ValuesAfter...>;
				};

			};

		};
	
	public:
		
		template<auto... Values>
		using for_values =
			typename values_before<>::
			template current_value_index<0>::
			template current_value_and_values_after<Values...>
			::type;

	}; // erase_at_index

}