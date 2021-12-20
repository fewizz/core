#pragma once

namespace type {

	struct is_reference {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type = false;

		template<typename Type>
		static constexpr bool for_type<Type&> = true;

		template<typename Type>
		static constexpr bool for_type<Type&&> = true;
	};
}