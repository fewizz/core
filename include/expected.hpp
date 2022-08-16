#pragma once

#include "./elements/one_of.hpp"

template<typename Type>
class unexpected {
	
};

template<typename Type, typename UnexpectedType>
class expected {
	elements::one_of<Type, UnexpectedType> one_of;

public:

	template<typename... Args>
	requires(
		!(sizeof...(Args) == 1 && types_are_same<expected, first_type<Args...>>)
	)
	constexpr expected(Args&&... args) :
		one_of{ forward<Args>(args)... }
	{}

	constexpr bool is_unexpected () const {
		return one_of.template is<UnexpectedType>();
	}

	constexpr bool is_expected () const {
		return one_of.template is<Type>();
	}

	constexpr const UnexpectedType& get_unexpected() const {
		return one_of.template get<UnexpectedType>();
	}

	constexpr UnexpectedType& get_unexpected() {
		return one_of.template get<UnexpectedType>();
	}

	constexpr const Type& get_expected() const {
		return one_of.template get<Type>();
	}

	constexpr Type& get_expected() {
		return one_of.template get<Type>();
	}

	constexpr operator Type& () {
		return get_expected();
	}

};