#pragma once

#include "../integer.hpp"
#include "../types/are_same.hpp"
#include "../types/count_of_type.hpp"
#include "../types/index_of_type.hpp"
#include "../type/remove_reference.hpp"
#include "../type/is_constructible_from.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../forward.hpp"
#include "../types/at_index.hpp"
#include "../move.hpp"

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename HeadType, typename... TailTypes>
	union recursive_one_of_elements_storage<HeadType, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;

		HeadType element;
		next_type next;

		template<typename... Args>
		requires(type::constructible_from<HeadType, Args&&...>)
		recursive_one_of_elements_storage(Args&&... args)
			: element{ forward<Args>(args)... }
		{}

		template<typename... Args>
		requires(! type::is_constructible_from<Args&&...>::template for_type_of<HeadType> && sizeof...(TailTypes) != 0)
		recursive_one_of_elements_storage(Args&&... args)
			: next{ forward<Args>(args)... }
		{}

		~recursive_one_of_elements_storage() {}

		void destruct(nuint index) {
			if(index == 0) {
				element.~HeadType();
			}
			else if constexpr(sizeof...(TailTypes) == 0) {
				throw;
			}
			else {
				next.destruct(index - 1);
			}
		}

		template<nuint Index>
		constexpr const auto& at() const {
			if constexpr(Index == 0u) return element;
			else {
				static_assert(Index > 0u);
				return next.template at<Index - 1u>();
			}
		}

		template<nuint Index>
		constexpr auto& at() {
			if constexpr(Index == 0u) return element;
			else {
				static_assert(Index > 0u);
				return next.template at<Index - 1u>();
			}
		}
	};

	template<typename HeadType, typename... TailTypes>
	union recursive_one_of_elements_storage<HeadType&, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;

		HeadType* element;
		next_type next;

		recursive_one_of_elements_storage(HeadType& ht)
			: element{ &ht }
		{}

		~recursive_one_of_elements_storage() {}

		void destruct(nuint index) {
			if(index == 0) {
				element = nullptr;
			}
			else if constexpr(sizeof...(TailTypes) == 0) {
				throw;
			}
			else {
				next.destruct(index - 1);
			}
		}

		template<typename... Args>
		requires(sizeof...(TailTypes) != 0)
		recursive_one_of_elements_storage(Args&&... args)
			: next{ forward<Args>(args)... }
		{}

		template<nuint Index>
		constexpr const auto& at() const {
			if constexpr(Index == 0u) return *element;
			else {
				static_assert(Index > 0u);
				return next.template at<Index - 1u>();
			}
		}

		template<nuint Index>
		constexpr auto& at() {
			if constexpr(Index == 0u) return *element;
			else {
				static_assert(Index > 0u);
				return next.template at<Index - 1u>();
			}
		}
	};

	template<typename... Types>
	struct one_of {
		recursive_one_of_elements_storage<Types...> m_storage;
		nuint m_current;
		static constexpr nuint no_value = -1;

		template<typename Type>
		static constexpr bool only_one_such_type =
			types::count_of_type<Type>::template for_types_of<Types...> == 1;

		template<nuint Index>
		using type_at = typename types::at_index<Index>::template for_types_of<Types...>;

		template<typename Type>
		static constexpr nuint type_index = types::index_of_type<Type>::template for_types_of<Types...>;

		template<typename... Args>
		requires(types::count_of_satisfying_predicate<type::is_constructible_from<Args&&...>>::template for_types_of<Types...> == 1)
		one_of(Args&&... element) :
			m_storage{ forward<Args>(element)... },
			m_current{
				types::index_of_satisfying_predicate<type::is_constructible_from<Args&&...>>::template for_types_of<Types...>
			}
		{}

		~one_of() {
			if(!no_value)
				m_storage.destruct(m_current);
		}

		template<typename Type>
		bool is_current() const {
			return m_current == type_index<Type>;
		}

		template<nuint Index>
		const type_at<Index>& at_index() const {
			if(m_current != Index) throw;
			return m_storage.template at<Index>(); 
		}

		template<nuint Index>
		type_at<Index>& at_index() {
			if(m_current != Index) throw;
			return m_storage.template at<Index>(); 
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		const Type& get() const {
			return at_index<type_index<Type>>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		Type& get() {
			return at_index<type_index<Type>>();
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		Type&& move() {
			m_current = no_value;
			return ::move(get<Type>());
		}
	};

}
