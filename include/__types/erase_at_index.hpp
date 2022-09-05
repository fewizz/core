#pragma once

#include "./of.hpp"

namespace __types {

	template <nuint Index>
	class erase_at_index {

		template<typename... TypesBefore>
		struct types_before {

			template<nuint CurrentIndex>
			struct current_type_index {

				template<typename CurrentValue, typename... TypesAfter>
				requires(sizeof...(TypesAfter) > 0)
				struct current_type_and_types_after {
					using type =
						typename types_before<TypesBefore..., CurrentValue>::
						template current_type_index<CurrentIndex + 1>::
						template current_type_and_types_after<TypesAfter...>
						::type;
				};

			};
	
			template<>
			struct current_type_index<Index> {

				template<typename CurrentValue, typename... TypesAfter>
				struct current_type_and_types_after {
					using type = __types::of<TypesBefore..., TypesAfter...>;
				};

			};

		};
	
	public:
		
		template<typename... Values>
		using for_types =
			typename types_before<>::
			template current_type_index<0>::
			template current_type_and_types_after<Values...>
			::type;

	};

}