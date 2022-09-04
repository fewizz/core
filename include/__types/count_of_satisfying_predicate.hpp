#pragma once

#include "./predicate.hpp"
#include "./indices_of_satisfying_predicate.hpp"
#include "../__type/predicate.hpp"
#include "../__type/modifier.hpp"
#include "../__type/decay.hpp"

namespace __types {

	template<nuint N>
	struct less : types_predicate_marker {
	};

	template<type_predicate Predicate>
	struct count_of_satisfying_predicate {

		template<typename... Types>
		static constexpr nuint value =
			(nuint(Predicate::template for_type<Types>) + ... + 0);

		constexpr auto operator < (nuint n) const {
			return less<n>{};
		}
		template<nuint N>
		struct less : types_predicate_marker {

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
		struct greater : types_predicate_marker {

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
		struct less_or_equals : types_predicate_marker {

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
		struct greater_or_equals : types_predicate_marker {

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
		struct equals : types_predicate_marker {

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