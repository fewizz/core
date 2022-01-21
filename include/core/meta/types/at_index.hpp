#pragma once

#include "of.hpp"
#include "../../integer.hpp"

namespace types {

	template<nuint Index>
	class at_index {
		template<nuint CurrentIndex, typename...>
		struct current_index_and_types_left;
	
		template<typename HeadType, typename... RemainingTypes>
		struct current_index_and_types_left<Index, HeadType, RemainingTypes...>
			: type::of<HeadType> {};
	
		template<nuint CurrentIndex, typename HeadType, typename... RemainingTypes>
		struct current_index_and_types_left<CurrentIndex, HeadType, RemainingTypes...>
			: type::of<typename current_index_and_types_left<CurrentIndex + 1, RemainingTypes...>::type>{};
	
	public:

		template<typename...Types>
		using for_types = typename current_index_and_types_left<0, Types...>::type;

	};

}