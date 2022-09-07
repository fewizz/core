#pragma once

template<typename Type, typename... Args>
concept invokable_with = requires(Type&& t, Args&&... args) {
	t(args...);
};

namespace __type {

	template<typename Type, typename... Args>
	constexpr inline bool is_invokable_with = invokable_with<Type, Args...>;

}