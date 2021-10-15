#pragma once

#include "../integer.hpp"
#include "at_index.hpp"
#include "forward.hpp"
#include "of.hpp"
#include "../values/are_contain_value.hpp"
#include "../types/at_indices.hpp"
#include "last.hpp"

namespace elements {

	template<uint... Indices>
	class at_indices {
		template<typename... ResultingTypes>
		struct resulting_types {

			static constexpr auto result(ResultingTypes&... elements) {
				return elements::of<ResultingTypes...> {
					forward<ResultingTypes>(elements)...
				};
			}
		};

		//template<uint CurrentIndex, typename Element>
		//auto for_current_index(Element&& elemen) {
		//	return elements::of {
		//		elements::at_index<Indices...>::for_elements_of()
		//	};
		//}

		/*template<uint CurrentIndex, typename... Types, typename... Resulting>
		auto for_current_index(of<Types>&&... elements, of<Resulting>&&... resulting) {
			if constexpr(values::are_contain_value<CurrentIndex>::template for_values_of<Indices...>) {
				auto& that_element = elements::last::for_elements_of(elements...);

				return for_current_index<CurrentIndex + 1u>(
					of<Types> &&elements...,
					Next &&next,
					Resulting &&resulting...
				)
			}
			return elements::of {
				elements::at_index<Indices...>::for_elements_of()
			};
		}*/
	public:

		template<typename... Types>
		static constexpr auto for_elements_of(Types&&... elements) {
			using resulting_types0 = typename types::at_indices<Indices...>::template for_types_of<Types...>::template pass_for_type_directly<
				resulting_types
			>;

			return resulting_types0::result(
					(elements::at_index<Indices>::for_elements_of(elements...))...
				);
		} 
	};

}