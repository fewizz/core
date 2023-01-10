#pragma once

#include "./variant.hpp"

template<typename Type, typename UnexpectedType>
class expected {
	variant<Type, UnexpectedType> one_of;

public:

	template<typename... Args>
	requires(
		!(sizeof...(Args) == 1 && types_are_same<expected, first_type<Args...>>)
	)
	constexpr expected(Args&&... args) :
		one_of{ forward<Args>(args)... }
	{}

	constexpr bool is_unexpected () const {
		return one_of.template is_same_as<UnexpectedType>();
	}

	constexpr bool is_expected () const {
		return one_of.template is_same_as<Type>();
	}

	constexpr const UnexpectedType&  get_unexpected() const &  {
		return one_of.template get_same_as<UnexpectedType>();
	}
	constexpr       UnexpectedType&  get_unexpected()       &  {
		return one_of.template get_same_as<UnexpectedType>();
	}
	constexpr const UnexpectedType&& get_unexpected() const && {
		return move(*this).one_of.template get_same_as<UnexpectedType>();
	}
	constexpr       UnexpectedType&& get_unexpected()       && {
		return move(*this).one_of.template get_same_as<UnexpectedType>();
	}

	constexpr const Type&  get_expected() const &  {
		return one_of.template get_same_as<Type>();
	}
	constexpr       Type&  get_expected()       &  {
		return one_of.template get_same_as<Type>();
	}
	constexpr const Type&& get_expected() const && {
		return move(*this).one_of.template get_same_as<Type>();
	}
	constexpr       Type&& get_expected()       && {
		return move(*this).one_of.template get_same_as<Type>();
	}

	template<typename Handler>
	const Type&  get_expected_or(Handler&& handler) const &  {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return get_expected();
	}
	template<typename Handler>
	      Type&  get_expected_or(Handler&& handler)       &  {
		if(is_unexpected()) { return handler(get_unexpected()); }
		return get_expected();
	}
	template<typename Handler>
	const Type&& get_expected_or(Handler&& handler) const && {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return move(*this).get_expected();
	}
	template<typename Handler>
	      Type&& get_expected_or(Handler&& handler)       && {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return move(*this).get_expected();
	}

};