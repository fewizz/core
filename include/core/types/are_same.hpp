#pragma once

namespace types {

	struct are_same {
		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		requires(sizeof...(Types) >= 2)
		static constexpr bool for_types_of = false;

		template<typename T>
		static constexpr bool for_types_of<T, T> = true;

		template<typename Head, typename... Tail>
		static constexpr bool for_types_of<Head, Head, Tail...> = for_types_of<Head, Tail...>;

	};

}

template<typename... Types>
inline constexpr bool types_are_same = types::are_same::for_types_of<Types...>;