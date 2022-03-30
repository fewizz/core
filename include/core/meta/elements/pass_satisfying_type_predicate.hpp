#pragma once

#include "at_index.hpp"
#include "of.hpp"
#include "../types/indices_of_satisfying_predicate.hpp"
#include "../types/at_indices.hpp"

namespace elements {

	template<type::predicate Predicate>
	struct pass_satisfying_type_predicate_t {

		template<typename... Types>
		struct acceptor {
			elements::of<Types...> elements;

			template<typename F>
			decltype(auto) operator () (F&& f) const {
				return elements.forward(forward<F>(f));
			}

			template<typename F>
			decltype(auto) operator () (F&& f) {
				return elements.forward(forward<F>(f));
			}
		};

		template<typename... Types>
		auto operator () (Types&&...elements) const {
			using indices = typename
				types::indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			using types = typename
				indices::template
				pass_for_type_directly<types::at_indices>::template
				for_types<Types...>;

			return
				[&]<nuint... Indices, typename... Types0>
				(::indices::of<Indices...>, ::types::of<Types0...>) {
				return acceptor<Types0...> {
					{
						forward<Types0>(
							elements::at_index<Indices>(elements...)
						)...
					}
				};
			}(indices{}, types{});
		}
	};

	template<type::predicate Predicate>
	inline constexpr auto pass_satisfying_type_predicate =
		elements::pass_satisfying_type_predicate_t<Predicate>
	{};

} // elements