#pragma once

#include "../type/decay.hpp"
#include "../integer.hpp"
#include "../forward.hpp"
#include "of.hpp"

namespace elements {

	template<nuint Index>
	struct at_index_t {

		template<typename HeadType, typename... TailTypes>
		constexpr decltype(auto) operator () (HeadType&& head_element, TailTypes&&... tail_elements) const {
			if constexpr(Index == 0) {
				return forward<HeadType>(head_element);
			}
			else {
				static_assert(sizeof...(TailTypes) > 0);
				return forward<
					typename types::at_index<Index>::template for_types_of<HeadType, TailTypes...>
				>(
					at_index_t<Index - 1>{}.template operator () <TailTypes...> (forward<TailTypes>(tail_elements)...)
				);
			}
		}
	};

	template<nuint Index>
	inline constexpr auto at_index = elements::at_index_t<Index>{};

}