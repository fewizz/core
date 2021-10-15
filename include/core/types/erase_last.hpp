#pragma once

#include "erase_at_index.hpp"

namespace types {

	struct erase_last {
		template<typename... Types>
		using for_types_of = typename types::erase_at_index<sizeof...(Types) - 1>::template for_types_of<Types...>;
	};
}