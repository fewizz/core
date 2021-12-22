#pragma once

#include "../integer.hpp"
#include "of.hpp"

namespace types {

	template<nuint Index>
	class at_index {
		template<nuint CurrentIndex, typename... Ts>
		struct current_index_and_types_left;
	
		template<typename Type, typename... Types>
		struct current_index_and_types_left<Index, Type, Types...> {
			using type = Type;
		};
	
		template<nuint CurrentIndex, typename Type, typename... Types>
		struct current_index_and_types_left<CurrentIndex, Type, Types...> {
			static_assert(sizeof...(Types) > 0, "out of bounds");

			using type
				= typename current_index_and_types_left<
					CurrentIndex + 1,
					Types...
				>::type;
		};
	
	public:

		template<typename...Types>
		using for_types_of = typename current_index_and_types_left<0, Types...>::type;

		template<typename>
		struct for_types_t;

		template<typename... Types>
		struct for_types_t<types::of<Types...>> {
			using type = for_types_of<Types...>;
		};

		template<typename TypesOf>
		using for_types = typename for_types_t<TypesOf>::type;
	};

}