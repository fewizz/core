#pragma once

#include "./predicate.hpp"
#include "./modifier.hpp"
#include "./remove_reference.hpp"
#include "./remove_const.hpp"

namespace type {

	template<type::predicate Predicate, type::modifier Modifier>
	struct modified_predicate : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type =
			Predicate::template for_type<
				typename Modifier::template for_type<Type>
			>;

	};

} // type