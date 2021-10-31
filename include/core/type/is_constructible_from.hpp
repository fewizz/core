#pragma once

namespace type {

	template<typename Type, typename... Types>
	concept constructible_from = requires(Types&&... args) {
		Type{ args... };
	};

	template<typename... Types>
	struct is_constructible_from {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = type::constructible_from<Type, Types...>;
	};
}