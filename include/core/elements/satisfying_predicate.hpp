#pragma once

#include "at_index.hpp"
#include "../forward.hpp"
#include "../types/indices_of_type.hpp"
#include "../types/count_of_satisfying_predicate.hpp"
#include "../types/at_index.hpp"
#include "../values/at_index.hpp"
#include "../types/index_of_satisfying_predicate.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct satisfying_predicate_t {
	
		template<typename... Types>
		requires(types::count_of_satisfying_predicate<Predicate>::template for_types_of<decay<Types>...> == 1)
		constexpr decltype(auto) operator () (Types&&... elements) const {
			constexpr auto index = types::index_of_satisfying_predicate<Predicate>::template for_types_of<decay<Types>...>;
			using type = typename types::at_index<index>::template for_types_of<Types...>;

			return 
				forward<type>(
					elements::at_index<index>(elements...)
				);
		}
	};

	template<type::predicate Predicate>
	inline constexpr auto satisfying_predicate = elements::satisfying_predicate_t<Predicate>{};
}