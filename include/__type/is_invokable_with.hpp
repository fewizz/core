#pragma once

template<typename Type, typename... Args>
concept invokable_with = requires(Type&& t, Args&&... args) {
	t(args...);
};

namespace __type {

	template<typename Type, typename... Args>
	constexpr inline bool is_invokable_with = invokable_with<Type, Args...>;

}

#include "./predicate_extensions.hpp"

template<typename... Args>
struct is_invokable_with_t :
	type_predicate_extensions<is_invokable_with_t<Args...>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return invokable_with<Type, Args...>;
	}

};

template<typename... Args>
static constexpr is_invokable_with_t<Args...> is_invokable_with{};