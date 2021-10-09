#pragma once

#include "at_indices.hpp"
#include "../values/of.hpp"

namespace types {


	template<typename Type>
	struct at;

	template<uint... Indices>
	struct at<indices::of<Indices...>> {

		template<typename... Types>
		using for_types_of = typename types::at_indices<Indices...>::template for_types_of<Types...>;
	};
}