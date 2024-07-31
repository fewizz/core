#pragma once

#include "./predicate.hpp"

template<typename Derived>
struct type_predicate_extension;


namespace __type {

	template<type_predicate auto A, type_predicate auto B>
	struct predicates_conjunction : type_predicate_extension<predicates_conjunction<A, B>> {
		constexpr type_predicate auto affecting_predicate() const {
			return A || B;
		}
		template<typename Type>
		constexpr bool for_type() const {
			return A.template for_type<Type>() && B.template for_type<Type>();
		}
	};

	template<type_predicate auto A, type_predicate auto B>
	struct predicates_disjunction : type_predicate_extension<predicates_disjunction<A, B>> {
		constexpr type_predicate auto affecting_predicate() const {
			return A || B;
		}
		template<typename Type>
		constexpr bool for_type() const {
			return A.template for_type<Type>() || B.template for_type<Type>();
		}
	};

	template<type_predicate auto Predicate>
	struct predicate_negation : type_predicate_extension<predicate_negation<Predicate>> {
		template<typename Type>
		constexpr bool for_type() const {
			return ! Predicate.template for_type<Type>();
		}
	};

}


#include "../__types/count_of_satisfying_predicate.hpp"


template<typename Derived>
struct type_predicate_extension_base : type_predicate_mark {

	constexpr auto operator && (this auto self, type_predicate auto other) {
		return __type::predicates_conjunction<self, other>{};
	}

	constexpr auto operator || (this auto self, type_predicate auto other) {
		return __type::predicates_disjunction<self, other>{};
	}

	constexpr auto operator ! (this auto self) {
		return __type::predicate_negation<self>{};
	}

	constexpr auto operator == (this auto self, nuint n) {
		return count_of_satisfying_predicate<self> == n;
	}

	constexpr auto operator <= (this auto self, nuint n) {
		return count_of_satisfying_predicate<self> <= n;
	}

	constexpr auto operator >= (this auto self, nuint n) {
		return count_of_satisfying_predicate<self> >= n;
	}

	constexpr auto operator < (this auto self, nuint n) {
		return count_of_satisfying_predicate<self> < n;
	}

	constexpr auto operator > (this auto self, nuint n) {
		return count_of_satisfying_predicate<self> > n;
	}

};


#include "./decay.hpp"


template<typename Predicate>
struct type_predicate_while_decayed_t :
	type_predicate_extension_base<type_predicate_while_decayed_t<Predicate>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return Predicate{}.template for_type<decay<Type>>();
	}

};



template<typename Derived>
struct type_predicate_extension : type_predicate_extension_base<Derived> {

	static constexpr type_predicate_while_decayed_t<Derived> while_decayed{};

	static constexpr type_predicate_while_decayed_t<Derived> decayed{};

};