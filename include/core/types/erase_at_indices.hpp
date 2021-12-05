#pragma once

#include "../integer.hpp"
#include "../values/are_contain_value.hpp"
#include "of.hpp"

namespace types {

	template<nuint... Indices>
	class erase_at_indices {
		template<nuint Index, typename...Types>
		struct index_and_types;

		template<nuint Index> // No types
		struct index_and_types<Index> {
			template<typename... ResultingTypes>
			struct resulting {
				using types = types::of<ResultingTypes...>;
			};
		};

		template<nuint Index, typename HeadType, typename... TailTypes>
		struct index_and_types<Index, HeadType, TailTypes...> {

			template<typename... ResultingTypes>
			struct resulting;

			static constexpr bool skip_index = values::are_contain_value<Index>::template for_values_of<Indices...>;

			template<typename... ResultingTypes>
			requires(skip_index)
			struct resulting<ResultingTypes...> {
				using types = typename index_and_types<Index + 1u, TailTypes...>::template resulting<ResultingTypes...>::types;
			};

			template<typename... ResultingTypes>
			requires(!skip_index)
			struct resulting<ResultingTypes...> {
				using types = typename index_and_types<Index + 1u, TailTypes...>::template resulting<ResultingTypes..., HeadType>::types;
			};
		};

	public:

		template<typename... Types>
		using for_types_of = typename index_and_types<0u, Types...>::template resulting<>::types;
	};
}