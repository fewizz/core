#pragma once

#include "of.hpp"
#include "../type/decay.hpp"
#include "../../forward.hpp"

namespace elements {

	template<nuint Index>
	struct at_index_t {

		template<typename... Types>
		constexpr decltype(auto) operator () (Types&&... elements) const {
			return elements::of{ forward<Types>(elements)... }.template at<Index>();
		}
	};

	template<nuint Index>
	inline constexpr auto at_index = elements::at_index_t<Index>{};

} // elements