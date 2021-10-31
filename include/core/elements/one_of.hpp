#pragma once

#include "../integer.hpp"
#include "../types/are_same.hpp"
#include "../types/count_of_type.hpp"
#include "../types/index_of_type.hpp"
#include "../type/remove_reference.hpp"
#include "../type/is_constructible_from.hpp"
#include "../types/index_of_satisfying_predicate.hpp"
#include "../forward.hpp"

namespace elements {

	template<typename... Types>
	union recursive_one_of_elements_storage {};

	template<typename HeadType, typename... TailTypes>
	union recursive_one_of_elements_storage<HeadType, TailTypes...> {
		using next_type = recursive_one_of_elements_storage<TailTypes...>;

		HeadType element;
		next_type next;

		template<typename... Args>
		requires(type::is_constructible_from<Args...>::template for_type_of<HeadType>)
		recursive_one_of_elements_storage(Args&&... args)
			: element{ forward<Args>(args)... }
		{}

		template<typename... Args>
		requires(! type::is_constructible_from<Args...>::template for_type_of<HeadType> && sizeof...(TailTypes) != 0)
		recursive_one_of_elements_storage(Args&&... args)
			: next{ forward<Args>(args)... }
		{}

		template<uint Index>
		constexpr auto& at() const {
			if constexpr(Index == 0u) return element;
			else {
				static_assert(Index > 0u);
				return next.template at<Index - 1u>();
			}
		}
	};

	template<typename... Types>
	struct one_of {
		recursive_one_of_elements_storage<Types...> m_storage;
		uint m_current;

		template<typename Type>
		static constexpr bool only_one_such_type =
			types::count_of_type<type::remove_reference::for_type_of<Type>>::template for_types_of<Types...> == 1;

		template<typename... Args>
		one_of(Args&&... element) :
			m_storage{ forward<Args>(element)... },
			m_current{
				types::index_of_satisfying_predicate<type::is_constructible_from<Args...>>::template for_types_of<Types...>
			}
		{}

		template<typename Type>
		bool is_current() const {
			return m_current == types::index_of_type<Type>::template for_types_of<Types...>;
		}

		template<uint Index>
		auto& at_index() const {
			return m_storage.template at<Index>(); 
		}

		template<typename Type>
		requires(only_one_such_type<Type>)
		auto& get() const {
			//if(!is_current<Type>()) {
			//	return nullptr;
			//}

			return at_index<types::index_of_type<Type>::template for_types_of<Types...>>();
		}
		//auto& get()
	};

}
