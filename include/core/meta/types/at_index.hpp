#pragma once

#include "of.hpp"
#include "../../integer.hpp"

namespace types {

	template<nuint Index>
	class at_index {

		template<nuint CurrentIndex, typename...>
		struct current_index_and_types_left;
	
		template<
			nuint CurrentIndex, typename HeadType, typename... RemainingTypes
		>
		struct current_index_and_types_left<
			CurrentIndex, HeadType, RemainingTypes...
		> :
			current_index_and_types_left<CurrentIndex - 1, RemainingTypes...>
		{};

		template<typename Type0, typename... Types>
		struct current_index_and_types_left<0, Type0, Types...> :
			type::of<Type0>
		{};

		template<typename Type0, typename Type1, typename... Types>
		struct current_index_and_types_left<1, Type0, Type1, Types...> :
			type::of<Type1>
		{};

		template<
			typename Type0, typename Type1, typename Type2, typename... Types
		>
		struct current_index_and_types_left<2, Type0, Type1, Type2, Types...> :
			type::of<Type2>
		{};

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename... Types
		>
		struct current_index_and_types_left<
			3,
			Type0, Type1, Type2, Type3, Types...
		> :
			type::of<Type3>
		{};

	public:

		template<typename...Types>
		using for_types =
			typename current_index_and_types_left<Index, Types...>::type;

	};

} // types