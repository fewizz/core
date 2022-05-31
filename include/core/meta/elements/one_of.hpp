#pragma once

#include "../types/are_same.hpp"
#include "../type/is_same_as.hpp"
#include "../type/is_assignable.hpp"
#include "../type/is_constructible_from.hpp"
#include "../types/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../types/are_contain_satisfying_predicate.hpp"
#include "../types/first.hpp"
#include "../../forward.hpp"
#include "../../placement_new.hpp"
#include "../type/is_trivial.hpp"
#include "../type/is_base.hpp"
#include "../type/conjuncted_predicates.hpp"
#include "../../move.hpp"

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type element_;
		next_type next_;

		constexpr recursive_one_of_elements_storage() {}

		template<typename... Args>
		requires type::is_constructible_from<Args...>::template for_type<Type>
		constexpr void init_raw(nuint index, Args&&... args) {
			if(index == 0) {
				new (&element_) Type(forward<Args>(args)...);
				return;
			}
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init_raw(index - 1, forward<Args>(args)...);
			}
		}

		template<typename... Args>
		constexpr void init_raw(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init_raw(index - 1, forward<Args>(args)...);
			}
		}

				template<typename... Args>
		requires type::is_constructible_from<Args...>::template for_type<Type>
		constexpr void init(nuint index, Args&&... args) {
			if(index == 0) {
				new (&element_) Type(forward<Args>(args)...);
				return;
			}
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init(index - 1, forward<Args>(args)...);
			}
		}

		template<typename... Args>
		constexpr void init(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init(index - 1, forward<Args>(args)...);
			}
		}

		constexpr ~recursive_one_of_elements_storage() {}

		// trivial recursive destructor
		constexpr void destruct(nuint index) {
			if(index == 0) {
				element_.~Type();
				return;
			}
			if constexpr(there_is_next) {
				next_.destruct(index - 1);
				next_.~next_type();
			}
		}

		template<typename Handler>
		decltype(auto) view_raw(nuint index, Handler&& handler) const {
			if(index == 0) {
				return handler(element_);
			}
			if constexpr(there_is_next) {
				return next_.view_raw(index - 1, forward<Handler>(handler));
			}
		}

		template<typename Handler>
		decltype(auto) view_raw(nuint index, Handler&& handler) {
			if(index == 0) {
				return handler(element_);
			}
			else if constexpr(there_is_next) {
				return next_.view_raw(index - 1, forward<Handler>(handler));
			}
		}

		template<typename Handler>
		decltype(auto) view(nuint index, Handler&& handler) const {
			return view_raw(index, forward<Handler>(handler));
		}

		template<typename Handler>
		decltype(auto) view(nuint index, Handler&& handler) {
			return view_raw(index, forward<Handler>(handler));
		}

		template<nuint Index> requires (Index == 0)
		constexpr const auto& at() const {
			return element_;
		}

		template<nuint Index> requires (Index == 0)
		constexpr auto& at() {
			return element_;
		}
	
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& at() const {
			return next_.template at<Index - 1>();
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& at() {
			return next_.template at<Index - 1>();
		}

	};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type&, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type* element_;
		next_type next_;

		constexpr recursive_one_of_elements_storage() {}

		constexpr void init_raw(nuint index, Type* ptr) {
			if(index == 0) {
				element_ = ptr;
				return;
			}
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init_raw(index - 1, ptr);
			}
		}

		template<typename... Args>
		constexpr void init_raw(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init_raw(index - 1, forward<Args>(args)...);
			}
		}

		constexpr void init(nuint index, Type& ref) {
			if(index == 0) {
				element_ = &ref;
				return;
			}
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init(index - 1, ref);
			}
		}

		template<typename... Args>
		constexpr void init(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next_) next_type();
				next_.init(index - 1, forward<Args>(args)...);
			}
		}

		constexpr ~recursive_one_of_elements_storage() {}

		constexpr void destruct(nuint index) {
			if(index == 0) {
				return;
			}
			if constexpr(there_is_next) {
				next_.destruct(index - 1);
				next_.~next_type();
			}
		}

		template<typename Handler>
		decltype(auto) view_raw(nuint index, Handler&& handler) const {
			if(index == 0) {
				return handler(element_);
			}
			if constexpr(there_is_next) {
				return next_.view_raw(index - 1, forward<Handler>(handler));
			}
		}

		template<typename Handler>
		decltype(auto) view_raw(nuint index, Handler&& handler) {
			if(index == 0) {
				return handler(element_);
			}
			if constexpr(there_is_next) {
				return next_.view_raw(index - 1, forward<Handler>(handler));
			}
		}

		template<typename Handler>
		decltype(auto) view(nuint index, Handler&& handler) const {
			return *view_raw(index, forward<Handler>(handler));
		}

		template<nuint Index> requires (Index == 0)
		constexpr const auto& at() const {
			return *element_;
		}

		template<nuint Index> requires (Index == 0)
		constexpr auto& at() {
			return *element_;
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& at() const {
			return next_.template at<Index - 1>();
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& at() {
			return next_.template at<Index - 1>();
		}

	};

	template<typename... Types>
	class one_of {
		using storage_type = recursive_one_of_elements_storage<Types...>;
		storage_type storage_;
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
		static constexpr bool has_one_copyable_and_constructible_from =
			types::are_contain_one_satisfying_predicate<
				type::conjuncted_predicates<
					type::is_constructible_from<Type>,
					type::is_assignable<Type>
				>
			>::template for_types<Types...>;

		template<typename Type>
		static constexpr nuint index_of_copyable_and_constructible_from_ =
			types::index_of_satisfying_predicate<
				type::conjuncted_predicates<
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
		template<typename... Args>
		requires has_one_constructible_from<Args...>
		constexpr one_of(Args&&... args) :
			current_ { index_of_constructible_from_args<Args...> }
		{
			storage_.init(current_, forward<Args>(args)...);
		}

		// copy constructor
		constexpr one_of(const one_of& other) :
			current_ { other.current_ }
		{
			other.view([&](auto& element) {
				storage_.init(current_, element);
			});
		}

		// move constructor
		constexpr one_of(one_of&& other) :
			current_ { other.current_ }
		{
			other.view([&](auto& element) {
				storage_.init(current_, move(element));
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
					storage_.init_raw(current_, move(element));
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
		requires has_one_copyable_and_constructible_from<TypeToAssign>
		constexpr one_of& operator = (TypeToAssign&& value) {
			nuint index = index_of_copyable_and_constructible_from_<TypeToAssign>;
			if(index == current_) {
				view_raw([&](auto& element) {
					if constexpr(
						type::is_assignable<
							decltype(value)
						>::template for_type<decltype(element)>
					) {
						element = value;
					}
				});
			}
			else {
				storage_.destruct(current_);
				current_ = index;
				storage_.init(index, forward<TypeToAssign>(value));
			}
			return *this;
		}

		constexpr ~one_of() { storage_.destruct(current_); }

		template<typename Handler>
		decltype(auto) view(Handler&& handler) const {
			return storage_.view(current_, move(handler));
		}

		template<typename Handler>
		decltype(auto) view(Handler&& handler) {
			return storage_.view(current_, move(handler));
		}

		template<typename Handler>
		decltype(auto) view_raw(Handler&& handler) const {
			return storage_.view_raw(current_, move(handler));
		}

		template<typename Handler>
		decltype(auto) view_raw(Handler&& handler) {
			return storage_.view_raw(current_, move(handler));
		}

		template<typename Type>
		constexpr bool is() const {
			return current_ == type_index<Type>;
		}

		template<nuint Index>
		constexpr type_at<Index>& at() {
			return storage_.template at<Index>();
		}

		template<nuint Index>
		constexpr const type_at<Index>& at() const {
			return storage_.template at<Index>();
		}

		template<typename Type>
		requires has_one_such_type<Type>
		constexpr const Type& get() const {
			return at<type_index<Type>>();
		}

		template<typename Type>
		requires has_one_such_type<Type>
		constexpr Type& get() {
			return at<type_index<Type>>();
		}

	}; // one_of

} // elements