#pragma once

#include <type_traits>

namespace are {

namespace internal {
	template<typename... Ts>
	struct are_same;

	template<typename T>
	struct are_same<T, T> {
		static constexpr bool value = true;
	};

	template<typename T0, typename T1, typename... Ts>
	struct are_same<T0, T1, Ts...> {
		static constexpr bool value = false;
	};

	template<typename T, typename... Ts>
	struct are_same<T, T, Ts...> {
		static constexpr bool value = are_same<T, Ts...>::value;
	};
}

template<typename... Ts>
struct types {
	template<typename T>
	using args_for_predicate = std::is_invocable<T, Ts...>;

	template<typename T>
	static constexpr bool args_for = std::is_invocable_v<T, Ts...>;

	using same_t = internal::are_same<Ts...>;
	
	static constexpr bool same = internal::are_same<Ts...>::value;
};

template<auto... Values>
struct values {
	static constexpr bool equal = (Values == ...);
};

}