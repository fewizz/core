#pragma once

#include "of.hpp"
#include "../values/of.hpp"
#include "../type/predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	class indices_of_satisfying_predicate {

		template<nuint CurrentIndex, nuint... Indices>
		struct current_index_and_resulting_indices {
		
			template<bool... Types>
			struct remaining;

			template<bool Current, bool... Remaining>
			requires(Current)
			struct remaining<Current, Remaining...> :
				type::of<
					typename current_index_and_resulting_indices<
						CurrentIndex + 1,
						Indices..., CurrentIndex
					>
					::template remaining<Remaining...>::type
				>
			{};

			template<bool Current, bool... Remaining>
			requires(!Current)
			struct remaining<Current, Remaining...> :
				type::of<
					typename current_index_and_resulting_indices<
						CurrentIndex + 1,
						Indices...
					>
					::template remaining<Remaining...>::type
				>
			{};

			template<>
			struct remaining<> :
				type::of<indices::of<Indices...>>
			{};

		};

	public:
	
		template<typename... Types>
		using for_types = typename
			current_index_and_resulting_indices<0>::template
			remaining<
				Predicate::template for_type<Types>...
			>::type;

	};

} // types