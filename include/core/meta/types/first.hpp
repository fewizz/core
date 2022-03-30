#pragma once

#include "../type/of.hpp"

namespace types {

	class first {

		template<typename FirstType, typename... RemainingTypes>
		struct for_types_t : type::of<FirstType> {};

	public:

		template<typename... Types>
		using for_types = typename for_types_t<Types...>::type;

	};

} // types