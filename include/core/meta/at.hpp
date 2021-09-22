#pragma once

#include "../integer.hpp"

template<uint Index>
class at {
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

	template<uint CurrentIndex, auto... Values>
	struct current_index_and_values_left;

	template<auto Value, auto... Values>
	struct current_index_and_values_left<Index, Value, Values...> {
		static constexpr auto value = Value;
	};

	template<uint CurrentIndex, auto Value, auto... Values>
	struct current_index_and_values_left<CurrentIndex, Value, Values...> {
		static constexpr auto value
			= current_index_and_values_left<
				CurrentIndex + 1u,
				Values...
			>::value;
	};

public:
	template<typename...Types>
	using of_types = typename current_index_and_types_left<0u, Types...>::type;

	template<auto...Values>
	static constexpr auto of_values = current_index_and_values_left<0u, Values...>::value;
}; // at