#pragma once

#include "../type/of.hpp"

namespace types {

	struct first {

		template<typename... Types>
		struct for_types_of_t;

		template<typename FirstType, typename... TailTypes>
		struct for_types_of_t<FirstType, TailTypes...> : type::of<FirstType> {};

		template<typename... Types>
		using for_types_of = typename for_types_of_t<Types...>::type;

	};

}