#pragma once

#include "meta/elements/one_of.hpp"

[[noreturn]] inline void default_unexpected_handler() {
	__builtin_trap();
}

constexpr void default_unexpected_handler(auto) {
	default_unexpected_handler();
}

template<typename Type, typename UnexpectedType, void(Handler)(UnexpectedType) = default_unexpected_handler>
class expected {
	elements::one_of<Type, UnexpectedType> one_of;
	bool handled = false;

public:

	template<typename... Args>
	requires(
		sizeof...(Args) != 1 ||
		!type::is_same_as<expected>::template for_type<types::first::for_types<Args...>>
	)
	constexpr expected(Args&&... expected) : one_of{ forward<Args>(expected)... } {}

	/*constexpr expected(expected&& other) :
		one_of{ move(other.one_of) },
		handled{ exchange(other.handled, true) }
	{}*/ // TODO

	constexpr ~expected() {
		handle_if_unexpected();
	}

	constexpr bool is_unexpected () const { return one_of.template is<UnexpectedType>(); }
	constexpr bool is_expected () const { return one_of.template is<Type>(); }

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
		handle_if_unexpected();
		return get_expected();
	}

	void set_handled(bool value = true) {
		handled = value;
	}

	void handle_if_unexpected(auto f) {
		if(is_unexpected() && !handled) {
			set_handled(true);
			f(get_unexpected());
		}
	}

	void handle_if_unexpected() {
		handle_if_unexpected(Handler);
	}
};