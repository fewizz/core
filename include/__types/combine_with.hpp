#pragma once

#include "./of.hpp"

namespace types {

	template<typename... TypesToCombineWith>
	struct combine_with {

		template<typename... Types>
		using for_types = types::of<Types..., TypesToCombineWith...>;

	};

} // types