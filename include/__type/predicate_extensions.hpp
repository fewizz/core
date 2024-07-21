#pragma once

#include "./predicate.hpp"

template<typename Derived>
struct type_predicate_extension;

template<type_predicate auto... Predicates>
struct type_predicates_conjunction_t :
	type_predicate_extension<type_predicates_conjunction_t<Predicates...>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return (Predicates.template for_type<Type>() && ...);
	}

};

template<type_predicate auto... Predicates>
constexpr inline type_predicates_conjunction_t<Predicates...>
	type_predicates_conjunction{};

template<type_predicate auto... Predicates>
struct type_predicates_disjunction_t :
	type_predicate_extension<type_predicates_disjunction_t<Predicates...>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return (Predicates.template for_type<Type>() && ...);
	}

};

template<type_predicate auto... Predicates>
constexpr inline type_predicates_disjunction_t<Predicates...>
	type_predicates_disjunction{};

template<type_predicate auto Predicate>
struct type_predicate_negation_t :
	type_predicate_extension<type_predicate_negation_t<Predicate>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return ! Predicate.template for_type<Type>();
	}

};

template<type_predicate auto Predicate>
constexpr inline type_predicate_negation_t<Predicate>
	type_predicate_negation{};


#include "../__types/count_of_satisfying_predicate.hpp"


template<typename Derived>
struct type_predicate_extension_base : type_predicate_marker {

	template<type_predicate OtherPredicate>
	constexpr auto operator && (OtherPredicate) const {
		return type_predicates_conjunction<Derived{}, OtherPredicate{}>;
	}

	template<type_predicate OtherPredicate>
	constexpr auto operator || (OtherPredicate) const {
		return type_predicates_disjunction<Derived{}, OtherPredicate{}>;
	}

	constexpr auto operator ! () const {
		return type_predicate_negation<Derived{}>;
	}

	constexpr auto operator == (nuint n) const {
		return count_of_satisfying_predicate<Derived{}> == n;
	}

	constexpr auto operator <= (nuint n) const {
		return count_of_satisfying_predicate<Derived{}> <= n;
	}

	constexpr auto operator >= (nuint n) const {
		return count_of_satisfying_predicate<Derived{}> >= n;
	}

	constexpr auto operator < (nuint n) const {
		return count_of_satisfying_predicate<Derived{}> < n;
	}

	constexpr auto operator > (nuint n) const {
		return count_of_satisfying_predicate<Derived{}> > n;
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