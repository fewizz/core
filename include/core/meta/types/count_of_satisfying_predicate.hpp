#pragma once

#include "predicate.hpp"
#include "indices_of_satisfying_predicate.hpp"
#include "../type/predicate.hpp"
#include "../type/modifier.hpp"
#include "../type/decay.hpp"

namespace types {

	template<type::predicate Predicate>
	struct count_of_satisfying_predicate {

		template<typename... Types>
		static constexpr nuint for_types =
			types::indices_of_satisfying_predicate<Predicate>
			::template for_types<Types...>
			::size;

		template<nuint N>
		struct less : types::predicate_marker {

			using affecting_predicate = Predicate;

			template<typename... Types>
			using indices_of_affected_types =
				typename indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			template<typename... Types>
			static constexpr bool for_types =
				count_of_satisfying_predicate<Predicate>::
				for_types<Types...> < N;

		};

		template<nuint N>
		struct greater : types::predicate_marker {

			using affecting_predicate = Predicate;

			template<typename... Types>
			using indices_of_affected_types =
				typename indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			template<typename... Types>
			static constexpr bool for_types =
				count_of_satisfying_predicate<Predicate>::
				for_types<Types...> > N;

		};

		template<nuint N>
		struct less_or_equals : types::predicate_marker {

			using affecting_predicate = Predicate;

			template<typename... Types>
			using indices_of_affected_types =
				typename indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			template<typename... Types>
			static constexpr bool for_types =
				count_of_satisfying_predicate<Predicate>::
				for_types<Types...> <= N;

		};

		template<nuint N>
		struct greater_or_equals : types::predicate_marker {

			using affecting_predicate = Predicate;

			template<typename... Types>
			using indices_of_affected_types =
				typename indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			template<typename... Types>
			static constexpr bool for_types =
				count_of_satisfying_predicate<Predicate>::
				for_types<Types...> >= N;

		};
	
		template<nuint N>
		struct equals : types::predicate_marker {

			using affecting_predicate = Predicate;

			template<typename... Types>
			using indices_of_affected_types =
				typename indices_of_satisfying_predicate<Predicate>::template
				for_types<Types...>;

			template<typename... Types>
			static constexpr bool for_types =
				count_of_satisfying_predicate<Predicate>::
				for_types<Types...> == N;

		};

	}; // count_of_satisfying_predicate

} // types