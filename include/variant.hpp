#pragma once

#include "./__variant/storage.hpp"

#include "./__type/is_same_as.hpp"
#include "./__type/is_assignable.hpp"
#include "./__type/is_constructible_from.hpp"
#include "./__type/is_list_constructible_from.hpp"
#include "./__type/is_trivial.hpp"
#include "./__type/is_base.hpp"
#include "./__values/first.hpp"

#include "./types.hpp"

#include "./forward.hpp"
#include "./placement_new.hpp"
#include "./move.hpp"
#include "./type.hpp"

template<typename... Types>
class variant {
	using storage_type = __variant::storage<Types...>;
	using index_type = uint_for_states_count<sizeof...(Types)>;
	storage_type storage_{};
	index_type current_;

	template<typename Type>
	static constexpr bool has_one_such_type =
		types<Types...>::template count_of_satisfying_predicate<
			is_same_as<Type>
		> == 1;

	template<typename... Args>
	static constexpr bool has_one_constructible_from =
		types<Types...>::template count_of_satisfying_predicate<
			is_constructible_from<Args...>
		> == 1;

	template<typename... Args>
	static constexpr bool has_one_list_constructible_from =
		types<Types...>::template count_of_satisfying_predicate<
			is_list_constructible_from<Args...>
		> == 1;

	template<typename... Args>
	static constexpr nuint index_of_constructible_from =
		types<Types...>::template index_of_satisfying_predicate<
			is_constructible_from<Args...>
		>;

	template<typename... Args>
	static constexpr nuint index_of_list_constructible_from =
		types<Types...>::template index_of_satisfying_predicate<
			is_list_constructible_from<Args...>
		>;

	template<typename Type>
	static constexpr bool has_one_assignable_and_constructible_from =
		types<Types...>::template count_of_satisfying_predicate<
			is_constructible_from<Type> && is_assignable<Type>
		> == 1;

	template<typename Type>
	static constexpr nuint index_of_assignable_and_constructible_from =
		types<Types...>::template index_of_satisfying_predicate<
			is_constructible_from<Type> && is_assignable<Type>
		>;

public:

	template<index_type Index>
	using type_at = type_at_index<Index, Types...>;

	template<typename Type>
	static constexpr nuint type_index =
		types<Types...>::template index_of_satisfying_predicate<
			is_same_as<Type>
		>;

	// constructor
	template<typename Type>
	requires has_one_constructible_from<Type>
	constexpr variant(Type&& arg) :
		current_ {
			index_of_constructible_from<Type>
		}
	{
		storage_.template init<__variant::treat_type_as::value_type>(
			current_, forward<Type>(arg)
		);
	}

	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		has_one_list_constructible_from<Type>
	)
	constexpr variant(Type&& arg) :
		current_ {
			index_of_list_constructible_from<Type>
		}
	{
		storage_.template init<__variant::treat_type_as::value_type>(
			current_, forward<Type>(arg)
		);
	}

	// copy constructor
	constexpr variant(const variant& other) :
		current_ { other.current_ }
	{
		other.view([&](auto& element) {
			storage_.template init<
				__variant::treat_type_as::internal_type
			>(other.current_, element);
		});
	}

	// move constructor
	constexpr variant(variant&& other) :
		current_ { other.current_ }
	{
		other.view([&](auto& element) {
			storage_.template init<
				__variant::treat_type_as::value_type
			>(current_, move(element));
		});
	}

	// move assignment operator
	constexpr variant& operator = (variant&& other) {
		if(current_ == other.current_) {
			view_raw([&](auto& this_e) {
				other.view_raw([&](auto& other_e) {
					if constexpr(
						assignable<
							decltype(this_e), decltype(move(other_e))
						>
					) {
						this_e = move(other_e);
					}
				});
			});
		}
		else {
			storage_.destruct(current_);
			current_ = other.current_;
			other.view_raw([&](auto& element) {
				storage_.template init<
					__variant::treat_type_as::internal_type
				>(current_, move(element));
			});
		}
		return *this;
	}

	// copy assignment operator
	constexpr variant& operator = (const variant& other) {
		if(current_ == other.current_) {
			view([&](auto& this_e) {
				other.view([&](auto& other_e) {
					if constexpr(
						assignable<
							decltype(this_e), decltype(other_e)
						>
					) {
						this_e = other_e;
					}
				});
			});
		}
		else {
			storage_.destruct(current_);
			current_ = other.current_;
			other.view_raw([&](auto& element) {
				storage_.init_raw(current_, element);
			});
		}
		return *this;
	}

	// assignment operator
	template<typename TypeToAssign>
	requires has_one_assignable_and_constructible_from<TypeToAssign>
	constexpr variant& operator = (TypeToAssign&& value) {
		nuint index =
			index_of_assignable_and_constructible_from<TypeToAssign>;
		if(index == current_) {
			view_raw([&](auto& element) {
				if constexpr(
					assignable<
						decltype(element),
						decltype(forward<TypeToAssign>(value))
					>
				) {
					element = forward<TypeToAssign>(value);
				}
			});
		}
		else {
			storage_.destruct(current_);
			current_ = index;
			storage_.template init<
				__variant::treat_type_as::value_type
			>(index, forward<TypeToAssign>(value));
		}
		return *this;
	}

	template<typename Type>
	requires (
		!has_one_assignable_and_constructible_from<Type> &&
		constructible_from<variant, Type>
	)
	constexpr variant& operator = (Type&& value) {
		(*this) = variant(forward<Type>(value));
		return *this;
	}

	constexpr ~variant() {
		storage_.destruct(current_);
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) const {
		return storage_.template view<
			__variant::treat_type_as::value_type
		>(
			current_, move(handler)
		);
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) {
		return storage_.template view<
			__variant::treat_type_as::value_type
		>(current_, move(handler));
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) const {
		return storage_.view_raw(current_, move(handler));
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) {
		return storage_.template view<
			__variant::treat_type_as::internal_type
		>(current_, move(handler));
	}

	template<typename Type>
	constexpr bool is() const {
		return current_ == type_index<Type>;
	}

	template<nuint Index>
	constexpr type_at<Index>& at() & {
		return storage_.template at<Index>();
	}

	template<nuint Index>
	constexpr const type_at<Index>& at() const & {
		return storage_.template at<Index>();
	}

	template<nuint Index>
	constexpr type_at<Index>&& at() && {
		return move(storage_.template at<Index>());
	}

	template<nuint Index>
	constexpr const type_at<Index>&& at() const && {
		return move(storage_.template at<Index>());
	}

	template<typename Type>
	requires has_one_such_type<Type>
	constexpr const Type& get() const & {
		return at<type_index<Type>>();
	}

	template<typename Type>
	requires has_one_such_type<Type>
	constexpr Type& get() & {
		return at<type_index<Type>>();
	}

	template<typename Type>
	requires has_one_such_type<Type>
	constexpr const Type&& get() const && {
		return move(at<type_index<Type>>());
	}

	template<typename Type>
	requires has_one_such_type<Type>
	constexpr Type&& get() && {
		return move(at<type_index<Type>>());
	}

};