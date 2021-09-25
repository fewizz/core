#pragma once

#include "indices_of_type.hpp"
#include "meta/erase_value_at.hpp"
#include "meta/types.hpp"

template<typename TypeToErase>
class erase_type {

	template<typename... Types>
	struct types;

	template<typename Type>
	struct types<Type> {
		template<typename... ResultTytpes>
		using result = types_of<Type, ResultTytpes...>;
	};

	template<>
	struct types<TypeToErase> {
		template<typename... ResultTytpes>
		using result = types_of<ResultTytpes...>;
	};

	template<typename HeadType, typename... TailTypes>
	struct types<HeadType, TailTypes...> {
		template<typename... ResultTytpes>
		using result = typename types<TailTypes...>::template result<HeadType, ResultTytpes...>;
	};

	template<typename... TailTypes>
	struct types<TypeToErase, TailTypes...> {
		template<typename... ResultTytpes>
		using result = typename types<TailTypes...>::template result<ResultTytpes...>;
	};
public:

	template<typename... Types>
	using for_types_of = typename types<Types...>::template result<>;
};