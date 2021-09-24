#pragma once

#include "../integer.hpp"

template<uint Index>
class type_at {
	template<uint CurrentIndex, typename... Ts>
	struct current_index_and_types_left;

	template<typename Type, typename... Types>
	struct current_index_and_types_left<Index, Type, Types...> {
		using type = Type;
	};

	template<uint CurrentIndex, typename Type, typename... Types>
	struct current_index_and_types_left<CurrentIndex, Type, Types...> {
		using type
			= typename current_index_and_types_left<
				CurrentIndex + 1u,
				Types...
			>::type;
	};

public:
	template<typename...Types>
	using for_types_of = typename current_index_and_types_left<0u, Types...>::type;
}; // at