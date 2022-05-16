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

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type element;
		next_type next;

		constexpr recursive_one_of_elements_storage() {}

		template<typename... Args>
		requires type::is_constructible_from<Args...>::template for_type<Type>
		constexpr void init(nuint index, Args&&... args) {
			if(index == 0) {
				new (&element) Type(forward<Args>(args)...);
			}
			else if constexpr(there_is_next) {
				new (&next) next_type();
				next.init(index - 1, forward<Args>(args)...);
			}
		}

		template<typename... Args>
		constexpr void init(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next) next_type();
				next.init(index - 1, forward<Args>(args)...);
			}
		}

		template<typename Other>
		requires type::is_assignable<Other>::template for_type<Type>
		constexpr void assign(nuint index, Other&& other) {
			if(index == 0) {
				element = forward<Other>(other);
			}
			else if constexpr(there_is_next) {
				next.assign(index - 1, forward<Other>(other));
			}
		}

		template<typename Other>
		constexpr void assign(nuint index, Other&& other) {
			if constexpr(there_is_next) {
				next.assign(index - 1, forward<Other>(other));
			}
		}

		constexpr ~recursive_one_of_elements_storage() {}

		// trivial recursive destructor
		constexpr void destruct(nuint index) {
			if(index == 0) {
				element.~Type();
			}
			else if constexpr(there_is_next) {
				next.destruct(index - 1);
				next.~next_type();
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

		constexpr recursive_one_of_elements_storage() {}

		constexpr void init(nuint index, Type& ref) {
			if(index == 0) {
				element = &ref;
			}
			else if constexpr(there_is_next) {
				new (&next) next_type();
				next.init(index - 1, ref);
			}
		}

		template<typename... Args>
		constexpr void init(nuint index, Args&&... args) {
			if constexpr(there_is_next) {
				new (&next) next_type();
				next.init(index - 1, forward<Args>(args)...);
			}
		}

		template<typename Other>
		requires type::is_assignable<Other>::template for_type<Type&>
		constexpr void assign(nuint index, Other&& other) {
			if(index == 0) {
				*element = forward<Other>(other);
			}
			else if constexpr(there_is_next) {
				next.assign(index - 1, forward<Other>(other));
			}
		}

		template<typename Other>
		constexpr void assign(nuint index, Other&& other) {
			if constexpr(there_is_next) {
				next.assign(index - 1, forward<Other>(other));
			}
		}

		constexpr ~recursive_one_of_elements_storage() {}

		constexpr void destruct(nuint index) {
			if(index == 0) {
			}
			else if constexpr(there_is_next) {
				next.destruct(index - 1);
				next.~next_type();
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

		using storage_type = recursive_one_of_elements_storage<Types...>;
		storage_type m_storage;
		nuint m_current;

		template<typename Type>
		static constexpr bool one_such_type =
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
			m_current { index_of_constructible_from_args<Args&&...> }
		{
			m_storage.init(m_current, forward<Args>(args)...);
		}

		// assignment operator
		template<typename Type>
		constexpr one_of& operator = (Type&& value) {
			nuint index = type_index<Type>;
			if(index == m_current) {
				m_storage.assign(index, forward<Type>(value));
			}
			else {
				m_storage.destruct(m_current);
				m_storage.init(index, forward<Type>(value));
				m_current = index;
			}
			return *this;
		}

		constexpr ~one_of() { m_storage.destruct(m_current); }

		template<typename Type>
		constexpr bool is() const {
			return m_current == type_index<Type>;
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
		requires one_such_type<Type>
		constexpr const Type& get() const {
			return at<type_index<Type>>();
		}

		template<typename Type>
		requires one_such_type<Type>
		constexpr Type& get() {
			return at<type_index<Type>>();
		}

	}; // one_of

} // elements