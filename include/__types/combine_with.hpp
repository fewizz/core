#pragma once

#include "./of.hpp"

namespace __types {

	template<typename... TypesToCombineWith>
	struct combine_with {

		template<typename... Types>
		using for_types = __types::of<Types..., TypesToCombineWith...>;

	};

}