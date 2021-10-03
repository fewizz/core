#pragma once

#include "types/of.hpp"

namespace types {

	template<typename TypeToErase>
	class erase_type {

		template<typename... Types>
		struct types_to_check;

		template<typename Type>
		struct types_to_check<Type> {
			template<typename... ResultTytpes>
			using remaining_types = types::of<Type, ResultTytpes...>;
		};

		template<>
		struct types_to_check<TypeToErase> {
			template<typename... ResultTytpes>
			using remaining_types = types::of<ResultTytpes...>;
		};

		template<typename HeadType, typename... TailTypes>
		struct types_to_check<HeadType, TailTypes...> {
			template<typename... ResultTytpes>
			using remaining_types = typename types_to_check<TailTypes...>::template remaining_types<HeadType, ResultTytpes...>;
		};

		template<typename... TailTypes>
		struct types_to_check<TypeToErase, TailTypes...> {
			template<typename... ResultTytpes>
			using remaining_types = typename types_to_check<TailTypes...>::template remaining_types<ResultTytpes...>;
		};
	public:

		template<typename... Types>
		using for_types_of = typename types_to_check<Types...>::template remaining_types<>;
	};

}