#pragma once

#include "../types/are_same.hpp"
#include "remove_reference.hpp"

namespace type {

	template<typename Type0>
	struct is_same_as {
		static constexpr bool is_type_predicate = true;
	
		template<typename Type1>
		static constexpr bool for_type_of = types::are_same::for_types_of<Type0, Type1>;

		struct remove_reference {
			static constexpr bool is_type_predicate = true;
			template<typename Type1>
			static constexpr bool for_type_of =
				types::are_same::for_types_of<
					type::remove_reference::for_type_of<Type0>,
					type::remove_reference::for_type_of<Type1>
				>;
		};
	};

}