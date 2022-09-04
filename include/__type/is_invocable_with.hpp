#pragma once

template<typename Type, typename... Args>
concept invocable_with = requires(Type&& t, Args&&... args) {
	t(args...);
};

namespace __type {

	template<typename Type, typename... Args>
	constexpr inline bool is_invocable_with = invocable_with<Type, Args...>;

}