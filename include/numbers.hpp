#pragma once

#include "./array.hpp"

template<typename Type, nuint Size>
struct numbers : array<Type, Size> {

	template<typename... Args>
	constexpr numbers(Args&&... args) :
		array<Type, Size>{ forward<Args>(args)... }
	{}

	constexpr auto max() const requires (Size >= 2) {
		auto it = this->iterator();
		Type result = *it++;
		while (it != this->end()) {
			Type v = *it++;
			if (v > result) {
				result = v;
			}
		}
		return result;
	}

	constexpr auto min() const requires (Size >= 2) {
		auto it = this->iterator();
		Type result = *it++;
		while (it != this->end()) {
			Type v = *it++;
			if (v < result) {
				result = v;
			}
		}
		return result;
	}

	constexpr auto mix(float a) const requires (Size == 2) {
		auto x = (*this)[0];
		auto y = (*this)[1];
		return x + (y - x) * a;
	}

};

template<typename... Types>
requires(sizeof...(Types) == 1 || types_are_same<remove_reference<Types>...>)
numbers(Types&&...)
	-> numbers<
		remove_reference<first_type<Types...>>,
		sizeof...(Types)
	>;