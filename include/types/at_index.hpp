#pragma once

#include "of.hpp"
#include "../integer.hpp"

namespace types {

	template<nuint Index>
	struct at_index {

		template<typename...Types>
		using for_types =
			__type_pack_element<Index, Types...>;

	};

} // types