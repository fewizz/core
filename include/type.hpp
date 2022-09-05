#pragma once

#include "./__type/add_pointer.hpp"
#include "./__type/decay.hpp"
#include "./__type/is_addition_assignable.hpp"
#include "./__type/is_array.hpp"
#include "./__type/is_assignable.hpp"
#include "./__type/is_base.hpp"
#include "./__type/is_const.hpp"
#include "./__type/is_constructible_from.hpp"
#include "./__type/is_enum.hpp"
#include "./__type/is_function.hpp"
#include "./__type/is_invocable_with.hpp"
#include "./__type/is_move_constructible.hpp"
#include "./__type/is_pointer.hpp"
#include "./__type/is_reference.hpp"
#include "./__type/is_same_as.hpp"
#include "./__type/is_same_as_predicate.hpp"
#include "./__type/is_trivial.hpp"
#include "./__type/remove_const.hpp"
#include "./__type/remove_extent.hpp"
#include "./__type/remove_pointer.hpp"
#include "./__type/remove_reference.hpp"
#include "./__type/remove_volatile.hpp"

template<typename Type>
struct type {

	using add_pointer = __type::add_pointer<Type>;
	using decay = decay<Type>;

	template<typename WhatToAssign>
	static constexpr bool is_addition_assignable
		= addition_assignable<Type, WhatToAssign>;
	
	static constexpr bool is_array = type_is_array<Type>;

	template<typename WhatToAssign>
	static constexpr bool is_assignable = assignable<Type, WhatToAssign>;

	template<typename Derived>
	static constexpr bool is_base_of = base_of<Type, Derived>;

	static constexpr bool is_const = __type::is_const<Type>;

	template<typename... Args>
	static constexpr bool is_constructible_from
		= constructible_from<Type, Args...>;

	static constexpr bool is_enum = __type::is_enum<Type>;

	static constexpr bool is_function = __type::is_function<Type>;

	template<typename... Args>
	static constexpr bool is_invocable_with
		= __type::is_invocable_with<Type, Args...>;

	static constexpr bool is_move_constructible
		= __type::is_move_constructible<Type>;

	static constexpr bool is_pointer = __type::is_pointer<Type>;

	static constexpr bool is_reference = __type::is_reference<Type>;

	template<typename OtherType>
	static constexpr bool is_same_as = __type::is_same_as<Type, OtherType>;

	static constexpr bool is_trivial = __type::is_trivial<Type>;

	using remove_const = remove_const<Type>;

	using remove_extent = remove_extent<Type>;

	using remove_pointer = remove_pointer<Type>;

	using remove_reference = remove_reference<Type>;

	using remove_volatile = remove_volatile<Type>;

};