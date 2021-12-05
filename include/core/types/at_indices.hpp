#pragma once

#include "../integer.hpp"
#include "at_index.hpp"
#include "of.hpp"

namespace types {

	template<nuint... Indices>
	struct at_indices {

		template<typename... Types>
		using for_types_of =
			types::of<
				typename types::at_index<Indices>::template for_types_of<Types...> ...
			>;
	};
}