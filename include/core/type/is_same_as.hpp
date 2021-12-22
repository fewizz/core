#pragma once

#include "../types/are_same.hpp"
#include "remove_reference.hpp"
#include "remove_const.hpp"
#include "modified_predicate.hpp"

namespace type {

	template<typename Type0>
	struct is_same_as {
		static constexpr bool is_type_predicate = true;
	
		template<typename Type1>
		static constexpr bool for_type = types::are_same::for_types_of<Type0, Type1>;

		template<type::modifier Modifier>
		using mod = type::modified_predicate<is_same_as<Type0>, Modifier>;

		using ignore_reference = type::modified_predicate<is_same_as<Type0>, type::remove_reference>;
		using ignore_const = type::modified_predicate<is_same_as<Type0>, type::remove_const>;
	};

}