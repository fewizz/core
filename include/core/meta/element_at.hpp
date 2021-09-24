#pragma once

#include "../integer.hpp"
#include "../forward.hpp"

template<uint Index>
struct element_at {

	template<typename HeadType, typename... TailTypes>
	static constexpr decltype(auto) for_elements_of(HeadType&& head_element, TailTypes&&... tail_elements) {
		if constexpr(Index == 0u) {
			return forward<HeadType>(head_element);
		}
		else {
			static_assert(sizeof...(TailTypes) > 0);
			return of(forward<TailTypes>(tail_elements)...);
		}
	}

};