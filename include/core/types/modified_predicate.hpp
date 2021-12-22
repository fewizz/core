#pragma once

#include "../type/modifier.hpp"
#include "predicate.hpp"
#include "../type/remove_reference.hpp"
#include "../type/remove_const.hpp"

namespace types {

	template<types::predicate Predicate, type::modifier Modifier>
	struct modified_predicate {
		static constexpr bool is_types_predicate = true;

		template<typename... Types>
		static constexpr bool for_types_of =
			Predicate::template for_types_of<
				typename Modifier::template for_type<Types>...
			>;

		template<typename... Types>
		using indices_of_affected_types_of =
			typename Predicate::template indices_of_affected_types_of<
				typename Modifier::template for_type<Types>...
			>;

		template<type::modifier OtherModifier>
		using mod = modified_predicate<modified_predicate<Predicate, Modifier>, OtherModifier>;

		using ignore_reference = modified_predicate<modified_predicate<Predicate, Modifier>, type::remove_reference>;
		using ignore_const = modified_predicate<modified_predicate<Predicate, Modifier>, type::remove_const>;
	};
}