#pragma once

#include "modified_predicate.hpp"

namespace type {

	template<typename BaseType>
	struct is_base {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type = __is_base_of(BaseType, Type);

		template<type::modifier Modifier>
		using mod = type::modified_predicate<is_base<BaseType>, Modifier>;

		using ignore_reference = type::modified_predicate<is_base<BaseType>, type::remove_reference>;
		using ignore_const = type::modified_predicate<is_base<BaseType>, type::remove_const>;
	};

}