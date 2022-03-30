#pragma once

#include "of.hpp"
#include "guarded_of.hpp"

#include "../meta/types/are_contain_satisfying_predicate.hpp"
#include "../meta/elements/satisfying_predicate.hpp"

template<typename Type, typename ObjectType>
concept possibly_guarded_handle_of =
	handle_of<Type, ObjectType> ||
	guarded_handle_of<Type, ObjectType>;

namespace type {

	template<typename ObjectType>
	struct is_possibly_guarded_handle_of : ::type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type =
			possibly_guarded_handle_of<Type, ObjectType>;

	};

} // type

namespace types {

	template<typename ObjectType>
	struct are_contain_one_possibly_guarded_handle_of :
		types::are_contain_one_satisfying_predicate<
			type::is_possibly_guarded_handle_of<ObjectType>
		>
	{};

	template<typename ObjectType>
	struct are_may_contain_one_possibly_guarded_handle_of :
		types::are_may_contain_one_satisfying_predicate<
			type::is_possibly_guarded_handle_of<ObjectType>
		>
	{};

	template<typename ObjectType>
	struct are_contain_possibly_guarded_handle_of :
		types::are_contain_satisfying_predicate<
			type::is_possibly_guarded_handle_of<ObjectType>
		>
	{};

} // types

namespace elements {

	template<typename ObjectType>
	inline constexpr auto possibly_guarded_handle_of =
		elements::satisfying_predicate<
			type::is_possibly_guarded_handle_of<ObjectType>
		>;

} // elements