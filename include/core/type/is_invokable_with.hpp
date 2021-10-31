#pragma once

namespace type {

	template<typename Type, typename... Args>
	concept invocable_with = requires(Type&& t, Args&&... args) {
		t(args...);
	};

	template<typename... Args>
	struct is_invokable_with {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = invocable_with<Type, Args...>;
	};

}