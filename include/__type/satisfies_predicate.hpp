#pragma once

#include "./predicate.hpp"
#include "./decay.hpp"

template<type_predicate auto Predicate>
struct satisfies_predicate_t :
	type_predicate_extension<satisfies_predicate_t<Predicate>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return Predicate.template for_type<Type>();
	}

	struct while_decayed_t : type_predicate_extension<while_decayed_t> {
		template<typename Type>
		constexpr bool for_type() const {
			return Predicate.template for_type<decay<Type>>();
		}
	};

	constexpr auto while_decayed() const {
		return while_decayed_t{};
	}

};