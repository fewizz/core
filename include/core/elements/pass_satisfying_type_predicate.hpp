#pragma once

#include "../type/predicate.hpp"
#include "../types/indices_of_satisfying_predicate.hpp"
#include "../types/at.hpp"
#include "at_index.hpp"
#include "of.hpp"

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
			using indices_t = typename types::indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;
			using types_t = typename types::at<indices_t>::template for_types_of<Types...>;

			return [&]<nuint... Indices, typename... Types0>(indices::of<Indices...>, types::of<Types0...>) {
				return acceptor<Types0...> {
					{ 	forward<Types0>(elements::at_index<Indices>(elements...))... }
				};
			}(indices_t{}, types_t{});
		}
	};

	template<type::predicate Predicate>
	inline constexpr auto pass_satisfying_type_predicate = elements::pass_satisfying_type_predicate_t<Predicate>{};
}