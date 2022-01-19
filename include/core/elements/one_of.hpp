#pragma once

#include "../types/are_same.hpp"
#include "../types/count_of_type.hpp"
#include "../types/index_of_type.hpp"
#include "../type/is_constructible_from.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../types/first.hpp"
#include "../forward.hpp"
#include "../types/at_index.hpp"

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type element;
		next_type next;

		template<typename... Args>
		requires(type::constructible_from<Type, Args&&...>)
		constexpr recursive_one_of_elements_storage(Args&&... args)
			: element{ forward<Args>(args)... }
		{}

		template<typename... Args>
		requires(! type::is_constructible_from<Args&&...>::template for_type<Type> && there_is_next)
		constexpr recursive_one_of_elements_storage(Args&&... args)
			: next{ forward<Args>(args)... }
		{}

		constexpr recursive_one_of_elements_storage& operator = (Type&& value) {
			element = move(value);
			return *this;
		}

		template<typename Other>
		requires(there_is_next)
		constexpr recursive_one_of_elements_storage& operator = (Other&& other) {
			next = move(other);
			return *this;
		}

		constexpr ~recursive_one_of_elements_storage() {}

		constexpr void destruct(nuint index) {
			if(index == 0) {
				element.~Type();
			}
			else if constexpr(there_is_next) {
				next.destruct(index - 1);
			}
		}

		template<nuint Index> requires (Index == 0)
		constexpr const auto& get_at() const { return element; }
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& get_at() const { return next.template get_at<Index - 1>(); }

		template<nuint Index> requires (Index == 0)
		constexpr auto& get_at() { return element; }
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& get_at() { return next.template get_at<Index - 1>(); }
	};

	template<typename Type, typename... TailTypes>
	union recursive_one_of_elements_storage<Type&, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;
		static constexpr bool there_is_next = sizeof...(TailTypes) > 0;

		Type* element;
		next_type next;

		constexpr recursive_one_of_elements_storage(Type& ref)
			: element{ &ref }
		{}

		template<typename... Args>
		requires(
			there_is_next && (
				sizeof...(Args) != 1 ||
				!type::is_same_as<Type&>::template for_type<types::first::for_types_of<Args...>>
			)
		)
		constexpr recursive_one_of_elements_storage(Args&&... args)
			: next{ forward<Args>(args)... }
		{}

		constexpr recursive_one_of_elements_storage& operator = (Type& value) {
			element = &value;
			return *this;
		}

		template<typename Other>
		requires(there_is_next)
		constexpr recursive_one_of_elements_storage& operator = (Other&& other) {
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
		constexpr const auto& get_at() const { return *element; }
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr const auto& get_at() const { return next.template at<Index - 1>(); }

		template<nuint Index> requires (Index == 0)
		constexpr auto& get_at() { return *element; }
		template<nuint Index> requires (Index > 0 && there_is_next)
		constexpr auto& get_at() { return next.template at<Index - 1>(); }
	};

	template<typename... Types>
	class one_of {
		template<typename...>
		static constexpr nuint max_size = max(sizeof(Types)...);

		template<typename... Ts>
		requires(sizeof...(Types) == 1)
		static constexpr nuint max_size<Ts...> = sizeof(types::first::for_types_of<Types...>);

		recursive_one_of_elements_storage<Types...> m_storage;
		nuint m_current;

		template<typename Type>
		static constexpr bool only_one_such_type =
			types::count_of_type<Type>::template for_types_of<Types...> == 1;

		template<typename... Args>
		static constexpr nuint index_of_constructible_from_args =
			types::index_of_satisfying_predicate<type::is_constructible_from<Args...>>::template for_types_of<Types...>;

	public:

		template<nuint Index>
		using type_at = typename types::at_index<Index>::template for_types_of<Types...>;

		template<typename Type>
		static constexpr nuint type_index = types::index_of_type<Type>::template for_types_of<Types...>;

		template<typename... Args>
		requires(types::count_of_satisfying_predicate<type::is_constructible_from<Args&&...>>::template for_types_of<Types...> == 1)
		constexpr one_of(Args&&... args) :
			m_storage { forward<Args>(args)... },
			m_current { index_of_constructible_from_args<Args&&...> }
		{}

		//one_of(one_of&& other) = default; TODO

		template<typename Type>
		constexpr one_of& operator = (Type& value) {
			m_storage.destruct(m_current);
			m_storage(forward<Type>(value));
			m_current = types::index_of_type<Type>::template for_types_of<Types...>;
			return *this;
		}

		constexpr ~one_of() { m_storage.destruct(m_current); }

		template<typename Type>
		constexpr bool is() const {
			return m_current == type_index<Type>;
		}

		template<nuint Index>
		constexpr type_at<Index>& get_at() {
			return m_storage.template get_at<Index>();
		}

		template<nuint Index>
		constexpr const type_at<Index>& get_at() const {
			return m_storage.template get_at<Index>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		constexpr const Type& get() const {
			return get_at<type_index<Type>>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		constexpr Type& get() {
			return get_at<type_index<Type>>();
		}
	};

}
