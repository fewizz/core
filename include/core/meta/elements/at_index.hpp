#pragma once

#include "of.hpp"
#include "../type/decay.hpp"
#include "../../forward.hpp"

namespace elements {

	template<nuint Index>
	struct at_index_t {

		template<typename HeadType>
		requires(Index == 0)
		constexpr decltype(auto) operator () (HeadType&& head_element, auto&&...) const {
			return forward<HeadType>(head_element);
		}

		template<typename HeadType, typename... TailTypes>
		requires(Index > 0)
		constexpr decltype(auto) operator () (HeadType&&, TailTypes&&... tail_elements) const {
			static_assert(sizeof...(TailTypes) > 0);
			return at_index_t<Index - 1>{}(forward<TailTypes>(tail_elements)...);
		}
	};

	template<nuint Index>
	inline constexpr auto at_index = elements::at_index_t<Index>{};

}