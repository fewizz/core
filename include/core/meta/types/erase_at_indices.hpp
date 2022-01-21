#pragma once

#include "of.hpp"
#include "../../integer.hpp"
#include "../../meta/values/are_contain_value.hpp"

namespace types {

	template<nuint... Indices>
	class erase_at_indices {

		template<nuint CurrentIndex, typename...Types>
		struct index_and_remaining_types;

		template<nuint CurrentIndex, typename CurrentType, typename... RemainingTypes>
		struct index_and_remaining_types<CurrentIndex, CurrentType, RemainingTypes...> {

			static constexpr bool erase = values::are_contain_value<CurrentIndex>::template for_values<Indices...>;

			template<typename... ResultingTypes>
			struct resulting_types;

			template<typename... ResultingTypes>
			requires(erase)
			struct resulting_types<ResultingTypes...>
				: type::of<
					typename index_and_remaining_types<CurrentIndex + 1, RemainingTypes...>::
					template resulting<ResultingTypes...>::
					types
				>{};

			template<typename... ResultingTypes>
			requires(!erase)
			struct resulting_types<ResultingTypes...>
				: type::of<
					typename index_and_remaining_types<CurrentIndex + 1, RemainingTypes...>::
					template resulting<ResultingTypes..., CurrentType>::
					types
				>{};

		};

		template<nuint CurrentIndex>
		struct index_and_remaining_types<CurrentIndex> {

			template<typename... ResultingTypes>
			struct resulting_types : type::of<types::of<ResultingTypes...>> {};

		};

	public:

		template<typename... Types>
		using for_types = typename index_and_remaining_types<0, Types...>::template resulting_types<>::type;

	};
}