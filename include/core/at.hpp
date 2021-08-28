#pragma once

#include <cstddef>

template<std::size_t Index>
class at {
	template<std::size_t CurrentIndex, typename... Ts>
	struct current_index_and_types_left;

	template<typename Type, typename... Types>
	struct current_index_and_types_left<Index, Type, Types...> {
		using type = Type;
	};

	template<std::size_t CurrentIndex, typename Type, typename... Types>
	struct current_index_and_types_left<CurrentIndex, Type, Types...> {
		using type
			= typename current_index_and_types_left<CurrentIndex + 1, Types...>::type;
	};

	template<std::size_t CurrentIndex, auto... Values>
	struct current_index_and_values_left;

	template<auto Value, auto... Values>
	struct current_index_and_values_left<Index, Value, Values...> {
		static constexpr auto value = Value;
	};

	template<std::size_t CurrentIndex, auto Value, auto... Values>
	struct current_index_and_values_left<CurrentIndex, Value, Values...> {
		static constexpr auto value
			= current_index_and_values_left<CurrentIndex + 1, Values...>::value;
	};

public:
	template<typename...Types>
	using of_types = current_index_and_types_left<0, Types...>;

	template<auto...Values>
	static constexpr auto of_values = current_index_and_values_left<0, Values...>::value;
}; // at