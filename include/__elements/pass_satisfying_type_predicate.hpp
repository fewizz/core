#pragma once

#include "./at_index.hpp"
#include "../tuple.hpp"
#include "../__types/indices_of_satisfying_predicate.hpp"
#include "../__types/at_indices.hpp"

namespace __elements {

	template<type_predicate auto Predicate>
	struct pass_satisfying_type_predicate_t {

		template<typename... Types>
		struct acceptor {
			tuple<Types...> elements;

			template<typename F>
			constexpr decltype(auto) operator () (F&& f) const {
				return elements.forward(forward<F>(f));
			}

			template<typename F>
			constexpr decltype(auto) operator () (F&& f) {
				return elements.forward(forward<F>(f));
			}
		};

		template<typename... Types>
		constexpr auto operator () (Types&&...elements) const {
			using indices = typename
				__types::indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			using types = typename
				indices::template
				pass_for_type_directly<__types::at_indices>::template
				for_types<Types...>;

			return
				[&]<nuint... Indices, typename... Types0>
				(::indices::of<Indices...>, ::__types::of<Types0...>) {
				return acceptor<Types0...> {
					{
						forward<Types0>(
							__elements::at_index<Indices>(elements...)
						)...
					}
				};
			}(indices{}, types{});
		}
	};

	template<type_predicate auto Predicate>
	inline constexpr auto pass_satisfying_type_predicate {
		__elements::pass_satisfying_type_predicate_t<Predicate>{}
	};

}