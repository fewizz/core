#pragma once

#include "../type/decay.hpp"
#include "../integer.hpp"
#include "../forward.hpp"
#include "of.hpp"

namespace elements {

	template<nuint Index>
	struct at_index_t {

		template<typename HeadType, typename... TailTypes>
		constexpr auto& for_elements_of(const HeadType& head_element, const TailTypes&... tail_elements) const {
			if constexpr(Index == 0) {
				return head_element;
			}
			else {
				static_assert(sizeof...(TailTypes) > 0);
				return at_index_t<Index - 1>{}.template for_elements_of(tail_elements...);
			}
		}
	};

	template<nuint Index>
	inline constexpr auto at_index = elements::at_index_t<Index>{};

}