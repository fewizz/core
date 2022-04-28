#pragma once

#include "../types/are_same.hpp"
#include "../type/is_same_as.hpp"
#include "../type/is_constructible_from.hpp"
#include "../types/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../types/are_contain_satisfying_predicate.hpp"
#include "../types/first.hpp"
#include "../../forward.hpp"

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type element;
		next_type next;

		// constructor

		template<typename... Args>
		requires constructible_from<Type, Args&&...>
		constexpr recursive_one_of_elements_storage(Args&&... args) :
			element{ forward<Args>(args)... }
		{}

		template<typename... Args>
		requires(!constructible_from<Type, Args&&...> && there_is_next)
		constexpr recursive_one_of_elements_storage(Args&&... args) :
			next{ forward<Args>(args)... }
		{}

		// assignment

		constexpr recursive_one_of_elements_storage&
		operator = (Type&& value) {
			element = forward<Type>(value);
			return *this;
		}

		template<typename Other>
		requires(there_is_next)
		constexpr recursive_one_of_elements_storage&
		operator = (Other&& other) {
			next = forward<Other>(other);
			return *this;
		}

		constexpr ~recursive_one_of_elements_storage() {}

		// trivial recursive destructor
		constexpr void destruct(nuint index) {
			if(index == 0) {
				element.~Type();
			}
			else if constexpr(there_is_next) {
				next.destruct(index - 1);
			}
		}

		template<nuint Index> requires (Index == 0)
		constexpr const auto& at() const {
			return element;
		}
	
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& at() const {
			return next.template at<Index - 1>();
		}

		template<nuint Index> requires (Index == 0)
		constexpr auto& at() {
			return element;
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& at() {
			return next.template at<Index - 1>();
		}

	};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type&, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type* element;
		next_type next;

		// constructor

		constexpr recursive_one_of_elements_storage(Type& ref) :
			element{ &ref }
		{}

		template<typename... Args>
		requires there_is_next && (
			sizeof...(Args) != 1 ||
			!types_are_same<Type&, types::first::for_types<Args...>>
		)
		constexpr recursive_one_of_elements_storage(Args&&... args) :
			next{ forward<Args>(args)... }
		{}

		// assignment

		constexpr recursive_one_of_elements_storage&
		operator = (Type& value) {
			element = &value;
			return *this;
		}

		template<typename Other>
		requires(there_is_next)
		constexpr recursive_one_of_elements_storage&
		operator = (Other&& other) {
			next = forward<Other>(other);
			return *this;
		}

		constexpr ~recursive_one_of_elements_storage(){}

		constexpr void destruct(nuint index) {
			if(index == 0) {
				element = nullptr;
			}
			else if constexpr(there_is_next) {
				next.destruct(index - 1);
			}
		}

		template<nuint Index> requires (Index == 0)
		constexpr const auto& at() const {
			return *element;
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& at() const {
			return next.template at<Index - 1>();
		}

		template<nuint Index> requires (Index == 0)
		constexpr auto& at() {
			return *element;
		}

		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& at() {
			return next.template at<Index - 1>();
		}

	};

	template<typename... Types>
	class one_of {

		recursive_one_of_elements_storage<Types...> m_storage;
		nuint dm_current;

		template<typename Type>
		static constexpr bool only_one_such_type =
			types::are_contain_one_satisfying_predicate<
				type::is_same_as<Type>
			>::template for_types<Types...>;

		template<typename... Args>
		static constexpr nuint index_of_constructible_from_args =
			types::index_of_satisfying_predicate<
				type::is_constructible_from<Args...>
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
		requires types::are_contain_one_satisfying_predicate<
			type::is_constructible_from<Args&&...>
		>::template for_types<Types...>
		constexpr one_of(Args&&... args) :
			m_storage { forward<Args>(args)... },
			dm_current { index_of_constructible_from_args<Args&&...> }
		{}

		// assignment operator

		template<typename Type>
		constexpr one_of& operator = (Type&& value) {
			m_storage.destruct(dm_current);
			m_storage = forward<Type>(value);
			dm_current = type_index<decay<Type>>; // TODO
			return *this;
		}

		constexpr ~one_of() { m_storage.destruct(dm_current); }

		template<typename Type>
		constexpr bool is() const {
			return dm_current == type_index<Type>;
		}

		template<nuint Index>
		constexpr type_at<Index>& at() {
			return m_storage.template at<Index>();
		}

		template<nuint Index>
		constexpr const type_at<Index>& at() const {
			return m_storage.template at<Index>();
		}

		template<typename Type>
		requires only_one_such_type<Type>
		constexpr const Type& get() const {
			return at<type_index<Type>>();
		}

		template<typename Type>
		requires only_one_such_type<Type>
		constexpr Type& get() {
			return at<type_index<Type>>();
		}

	}; // one_of

} // elements