#pragma once

#include "elements/one_of.hpp"

extern "C" void abort();

constexpr void default_unexpected_handler(auto) {
	abort();
} 

template<typename Type, typename UnexpectedType, void(Handler)(UnexpectedType) = default_unexpected_handler>
class expected {
	elements::one_of<Type, UnexpectedType> one_of;
	bool handled = false;

public:

	template<typename... Args>
	constexpr expected(Args&&... expected) : one_of{ forward<Args>(expected)... } {}

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

	void set_handled(bool value) {
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