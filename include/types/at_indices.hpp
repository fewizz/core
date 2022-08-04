#pragma once

#include "of.hpp"
#include "../integer.hpp"

namespace types {

	template<nuint... Indices>
	struct at_indices {

		template<typename... Types>
		using for_types =
			types::of<
				__type_pack_element<Indices, Types...> ...
			>;

	};

} // types