#pragma once

#include "../type/of.hpp"

namespace types {

	struct first {
		template<typename... Types>
		struct for_types_of;

		template<typename FirstType, typename... TailTypes>
		struct for_types_of<FirstType, TailTypes...> : type::of<FirstType> {};
	};

}