#pragma once

#include "./predicate.hpp"
#include "../__type/predicate.hpp"

template<typename Derived>
struct types_predicate_extension;


namespace __types {

	template<types_predicate A, types_predicate B>
	struct predicates_conjunction : types_predicate_extension<predicates_conjunction<A, B>> {
		A a; B b;
		constexpr type_predicate auto affecting_predicate() const {
			return a.affecting_predicate() || b.affecting_predicate();
		}
		template<typename... Types>
		constexpr bool for_types() const {
			return a.template for_types<Types...>() && b.template for_types<Types...>();
		}
	};

	template<types_predicate A, types_predicate B>
	struct predicates_disjunction : types_predicate_extension<predicates_disjunction<A, B>> {
		A a; B b;
		constexpr type_predicate auto affecting_predicate() const {
			return a.affecting_predicate() || b.affecting_predicate();
		}
		template<typename... Types>
		constexpr bool for_types() const {
			return a.template for_types<Types...>() || b.template for_types<Types...>();
		}
	};

	template<types_predicate Predicate>
	struct predicate_negation : types_predicate_extension<predicate_negation<Predicate>> {
		Predicate p;
		template<typename... Types>
		constexpr bool for_types() const {
			return ! p.template for_types<Types...>();
		}
	};

}


template<typename Derived>
struct types_predicate_extension : types_predicate_mark {

	constexpr auto operator && (this auto self, types_predicate auto other) {
		return __types::predicates_conjunction{.a=self, .b=other};
	}

	constexpr auto operator || (this auto self, types_predicate auto other) {
		return __types::predicates_disjunction{.a=self, .b=other};
	}

	constexpr auto operator ! (this auto self) {
		return __types::predicate_negation{.p=self};
	}

};