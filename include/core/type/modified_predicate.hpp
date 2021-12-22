#pragma once

#include "predicate.hpp"
#include "modifier.hpp"
#include "remove_reference.hpp"
#include "remove_const.hpp"

namespace type {

	template<type::predicate Predicate, type::modifier Modifier>
	struct modified_predicate {
		static constexpr bool is_type_predicate = true;

		template<typename Type>
		static constexpr bool for_type = Predicate::template for_type<typename Modifier::template for_type<Type>>;

		template<type::modifier OtherModifier>
		using mod = modified_predicate<modified_predicate<Predicate, Modifier>, OtherModifier>;

		using ignore_reference = modified_predicate<modified_predicate<Predicate, Modifier>, type::remove_reference>;
		using ignore_const = modified_predicate<modified_predicate<Predicate, Modifier>, type::remove_const>;
	};

}