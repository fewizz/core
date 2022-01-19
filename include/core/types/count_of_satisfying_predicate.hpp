#pragma once

#include "../type/predicate.hpp"
#include "../type/modifier.hpp"
#include "../type/decay.hpp"
#include "predicate.hpp"
#include "indices_of_satisfying_predicate.hpp"

namespace types {

	template<type::predicate Predicate>
	struct count_of_satisfying_predicate {

		template<typename... Types>
		static constexpr nuint for_types_of =
			types::indices_of_satisfying_predicate<Predicate>
			::template for_types_of<Types...>
			::size;

		template<nuint N>
		struct less_or_equals : types::predicate_marker {
			template<typename... Types>
			using indices_of_affected_types_of = typename indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;

			template<typename... Types>
			static constexpr bool for_types_of = count_of_satisfying_predicate<Predicate>::for_types_of<Types...> <= N;
		};

		template<nuint N>
		struct greater_or_equals : types::predicate_marker {
			template<typename... Types>
			using indices_of_affected_types_of = typename indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;

			template<typename... Types>
			static constexpr bool for_types_of = count_of_satisfying_predicate<Predicate>::for_types_of<Types...> >= N;
		};
	
		template<nuint N>
		struct equals : types::predicate_marker {
			template<typename... Types>
			using indices_of_affected_types_of = typename indices_of_satisfying_predicate<Predicate>::template for_types_of<Types...>;

			template<typename... Types>
			static constexpr bool for_types_of = count_of_satisfying_predicate<Predicate>::for_types_of<Types...> == N;
		};
	};
}