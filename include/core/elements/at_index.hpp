#pragma once

#include "../integer.hpp"
#include "../forward.hpp"

namespace elements {

	template<uint Index>
	struct at_index {

		template<typename HeadType, typename... TailTypes>
		static constexpr decltype(auto) for_elements_of(HeadType&& head_element, TailTypes&&... tail_elements) {
			if constexpr(Index == 0u) {
				return forward<HeadType>(head_element);
			}
			else {
				static_assert(sizeof...(TailTypes) > 0);
				return at_index<Index - 1u>::for_elements_of(forward<TailTypes>(tail_elements)...);
			}
		}

	};

}