#pragma once

#include "predicate.hpp"

template<typename Type, typename... Args>
concept invocable_with = requires(Type&& t, Args&&... args) {
	t(args...);
};

namespace type {

	template<typename... Args>
	struct is_invokable_with : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = invocable_with<Type, Args...>;

	};

} // type