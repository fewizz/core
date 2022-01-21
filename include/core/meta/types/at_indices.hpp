#pragma once

#include "of.hpp"
#include "at_index.hpp"
#include "../../integer.hpp"

namespace types {

	template<nuint... Indices>
	struct at_indices {

		template<typename... Types>
		using for_types =
			types::of<
				typename types::at_index<Indices>::template for_types<Types...> ...
			>;

	};
}