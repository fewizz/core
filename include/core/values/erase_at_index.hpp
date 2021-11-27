#pragma once

#include "../integer.hpp"
#include "../values/of.hpp"

namespace values {
	template <uint Index>
	class erase_at_index {
	
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
					using type = values::of<LeftValues..., RightValues...>;
				};
			};
		};
	
	public:
		
		template<auto... Values>
		using for_values_of = typename left_values<>::template current_value_index<0>::template right_values<Values...>::type;
	};

}