#pragma once

#include "../tuple.hpp"
#include "../__types/indices_of_satisfying_predicate.hpp"

namespace __elements {

	template<type_predicate auto Predicate>
	struct for_each_satisfying_type_predicate_t {

		template<typename... Types>
		struct acceptor {
			tuple<Types...> elements;

			template<typename F>
			void operator () (F&& f) {
				elements.for_each(
					forward<F>(f),
					typename __types::indices_of_satisfying_predicate<
						Predicate
					>::template for_types<Types...>{}
				);
			}
		};

		template<typename... Types>
		auto operator () (Types&&... elements) const {
			return acceptor<Types...> {
				{ forward<Types>(elements)... }
			};
		}

	};

	template<type_predicate auto Predicate>
	inline constexpr auto for_each_satisfying_type_predicate {
		__elements::for_each_satisfying_type_predicate_t<Predicate>{}
	};

}