#pragma once

namespace type {

	struct is_reference {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = false;

		template<typename Type>
		static constexpr bool for_type_of<Type&> = true;

		template<typename Type>
		static constexpr bool for_type_of<Type&&> = true;
	};
}