#pragma once

#include "./predicate.hpp"
#include "./indices_of_satisfying_predicate.hpp"
#include "../__type/predicate.hpp"
#include "../__type/modifier.hpp"
#include "../__type/decay.hpp"


template<type_predicate auto Predicate>
struct count_of_satisfying_predicate_t {

	template<typename... Types>
	static constexpr nuint for_types
		= (nuint(Predicate.template for_type<Types>()) + ... + 0);

	struct less_than : types_predicate_marker {
		nuint n;

		static constexpr auto affecting_predicate = Predicate;

		template<typename... Types>
		using indices_of_affected_types = typename
			__types::indices_of_satisfying_predicate<Predicate>::template
			for_types<Types...>;

		template<typename... Types>
		constexpr bool for_types() const {
			return count_of_satisfying_predicate_t<Predicate>::
				for_types<Types...> < n;
		}

	};

	constexpr auto operator < (nuint n) const {
		return less_than{ .n = n };
	}

	struct greater_than : types_predicate_marker {
		nuint n;

		static constexpr auto affecting_predicate = Predicate;

		template<typename... Types>
		using indices_of_affected_types = typename
			__types::indices_of_satisfying_predicate<Predicate>::template
			for_types<Types...>;

		template<typename... Types>
		constexpr bool for_types() const {
			return count_of_satisfying_predicate_t<Predicate>::
				for_types<Types...> > n;
		};

	};

	constexpr auto operator > (nuint n) const {
		return greater_than{ .n = n };
	}

	struct less_than_or_equals_to : types_predicate_marker {
		nuint n;

		static constexpr auto affecting_predicate = Predicate;

		template<typename... Types>
		using indices_of_affected_types = typename
			__types::indices_of_satisfying_predicate<Predicate>::template
			for_types<Types...>;

		template<typename... Types>
		constexpr bool for_types() const {
			return count_of_satisfying_predicate_t<Predicate>::
				for_types<Types...> <= n;
		}

	};

	constexpr auto operator <= (nuint n) const {
		return less_than_or_equals_to{ .n = n };
	}

	struct greater_than_or_equals_to : types_predicate_marker {
		nuint n;

		static constexpr auto affecting_predicate = Predicate;

		template<typename... Types>
		using indices_of_affected_types = typename
			__types::indices_of_satisfying_predicate<Predicate>::template
			for_types<Types...>;

		template<typename... Types>
		constexpr bool for_types() const {
			return count_of_satisfying_predicate_t<Predicate>::
				for_types<Types...> >= n;
		}

	};

	constexpr auto operator >= (nuint n) const {
		return greater_than_or_equals_to{ .n = n };
	}

	struct equals_to : types_predicate_marker {
		nuint n;

		static constexpr auto affecting_predicate = Predicate;

		template<typename... Types>
		using indices_of_affected_types = typename
			__types::indices_of_satisfying_predicate<Predicate>::template
			for_types<Types...>;

		template<typename... Types>
		constexpr bool for_types() const {
			return count_of_satisfying_predicate_t<Predicate>::
				for_types<Types...> == n;
		}

	};

	constexpr auto operator == (nuint n) const {
		return equals_to{ .n = n };
	}

};

template<type_predicate auto Predicate>
constexpr inline count_of_satisfying_predicate_t<Predicate>
	count_of_satisfying_predicate{};

template<type_predicate auto Predicate>
constexpr inline count_of_satisfying_predicate_t<Predicate>
	count_of{};