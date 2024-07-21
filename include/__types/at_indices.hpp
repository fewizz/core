#pragma once

#include "./of.hpp"
#include "../__integer/integer.hpp"

namespace __types {

	template<nuint... Indices>
	struct at_indices {

		template<typename... Types>
		using for_types =
			__types::of<
				__type_pack_element<Indices, Types...> ...
			>;

	};

}