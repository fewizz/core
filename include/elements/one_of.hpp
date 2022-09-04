#pragma once

#include "./one_of_storage.hpp"

#include "../__type/is_same_as.hpp"
#include "../__type/is_assignable.hpp"
#include "../__type/is_constructible_from.hpp"
#include "../__type/is_trivial.hpp"
#include "../__type/is_base.hpp"
#include "../__type/predicates_conjunction.hpp"

#include "../types/are_same.hpp"
#include "../types/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../types/are_contain_satisfying_predicate.hpp"
#include "../types/first.hpp"

#include "../decayed_same_as.hpp"
#include "../forward.hpp"
#include "../placement_new.hpp"
#include "../move.hpp"

namespace elements {

	template<typename... Types>
	class one_of {
		using storage_type = one_of_storage<Types...>;
		storage_type storage_{};
		nuint current_;

		template<typename Type>
		static constexpr bool has_one_such_type =
			types::are_contain_one_satisfying_predicate<
				type::is_same_as<Type>
			>::template for_types<Types...>;

		template<typename... Args>
		static constexpr bool has_one_constructible_from =
			types::are_contain_one_satisfying_predicate<
				type::is_constructible_from<Args...>
			>::template for_types<Types...>;

		template<typename... Args>
		static constexpr nuint index_of_constructible_from_args =
			types::index_of_satisfying_predicate<
				type::is_constructible_from<Args...>
			>::template for_types<Types...>;

		template<typename Type>
		static constexpr bool has_one_assignable_and_constructible_from =
			types::are_contain_one_satisfying_predicate<
				type::predicates_conjunction<
					type::is_constructible_from<Type>,
					type::is_assignable<Type>
				>
			>::template for_types<Types...>;

		template<typename Type>
		static constexpr nuint index_of_assignable_and_constructible_from =
			types::index_of_satisfying_predicate<
				type::predicates_conjunction<
					type::is_constructible_from<Type>,
					type::is_assignable<Type>
				>
			>::template for_types<Types...>;

	public:

		template<nuint Index>
		using type_at = typename
			types::at_index<
				Index
			>::template for_types<Types...>;

		template<typename Type>
		static constexpr nuint type_index =
			types::index_of_satisfying_predicate<
				type::is_same_as<Type>
			>::template for_types<Types...>;

		// constructor
		template<typename Type>
		//requires has_one_constructible_from<Args...>
		requires types::are_contain_one_decayed_same_as<
			decay<Type>
		>::template for_types<Types...>
		constexpr one_of(Type&& arg) :
			current_ {
				types::index_of_decayed_same_as<
					decay<Type>
				>::template for_types<Types...>
			}
		{
			storage_.template init<one_of_storage_treat_type_as::value_type>(
				current_, forward<Type>(arg)
			);
		}

		// copy constructor
		constexpr one_of(const one_of& other) :
			current_ { other.current_ }
		{
			other.view([&](auto& element) {
				storage_.template init<
					one_of_storage_treat_type_as::internal_type
				>(other.current_, element);
			});
		}

		// move constructor
		constexpr one_of(one_of&& other) :
			current_ { other.current_ }
		{
			other.view([&](auto& element) {
				storage_.template init<
					one_of_storage_treat_type_as::value_type
				>(current_, move(element));
			});
		}

		// move assignment operator
		constexpr one_of& operator = (one_of&& other) {
			if(current_ == other.current_) {
				view_raw([&](auto& this_e) {
					other.view_raw([&](auto& other_e) {
						if constexpr(
							type::is_assignable<
								decltype(move(other_e))
							>::template for_type<decltype(this_e)>
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
						one_of_storage_treat_type_as::internal_type
					>(current_, move(element));
				});
			}
			return *this;
		}

		// copy assignment operator
		constexpr one_of& operator = (const one_of& other) {
			if(current_ == other.current_) {
				view([&](auto& this_e) {
					other.view([&](auto& other_e) {
						if constexpr(
							type::is_assignable<
								decltype(other_e)
							>::template for_type<decltype(this_e)>
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
		constexpr one_of& operator = (TypeToAssign&& value) {
			nuint index =
				index_of_assignable_and_constructible_from<TypeToAssign>;
			if(index == current_) {
				view_raw([&](auto& element) {
					if constexpr(
						type::is_assignable<
							TypeToAssign
						>::template for_type<decltype(element)>
					) {
						element = forward<TypeToAssign>(value);
					}
				});
			}
			else {
				storage_.destruct(current_);
				current_ = index;
				storage_.template init<
					one_of_storage_treat_type_as::value_type
				>(index, forward<TypeToAssign>(value));
			}
			return *this;
		}

		template<typename Type>
		requires (
			!has_one_assignable_and_constructible_from<Type> &&
			constructible_from<one_of, Type>
		)
		constexpr one_of& operator = (Type&& value) {
			(*this) = one_of{ forward<Type>(value) };
			return *this;
		}

		constexpr ~one_of() {
			storage_.destruct(current_);
		}

		template<typename Handler>
		decltype(auto) view(Handler&& handler) const {
			return storage_.template view<
				one_of_storage_treat_type_as::value_type
			>(
				current_, move(handler)
			);
		}

		template<typename Handler>
		decltype(auto) view(Handler&& handler) {
			return storage_.template view<
				one_of_storage_treat_type_as::value_type
			>(current_, move(handler));
		}

		template<typename Handler>
		decltype(auto) view_raw(Handler&& handler) const {
			return storage_.view_raw(current_, move(handler));
		}

		template<typename Handler>
		decltype(auto) view_raw(Handler&& handler) {
			return storage_.template view<
				one_of_storage_treat_type_as::internal_type
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

	}; // one_of

} // elements