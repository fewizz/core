#pragma once

#include "./predicate.hpp"
#include "./modifier.hpp"
#include "./remove_reference.hpp"
#include "./remove_const.hpp"

template<type_predicate Predicate, type_modifier Modifier>
struct modified_type_predicate : type_predicate_marker {

	template<typename Type>
	static constexpr bool value =
		Predicate::template for_type<
			typename Modifier::template for_type<Type>
		>;

};