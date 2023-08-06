#pragma once

#include "./array.hpp"

template<nuint Index, typename Type, nuint Size>
constexpr const Type&  get(const array<Type, Size>&  a) { return a[Index]; }

template<nuint Index, typename Type, nuint Size>
constexpr       Type&  get(      array<Type, Size>&  a) { return a[Index]; }

template<nuint Index, typename Type, nuint Size>
constexpr const Type&& get(const array<Type, Size>&& a) {
	return move(a[Index]);
}

template<nuint Index, typename Type, nuint Size>
constexpr       Type&& get(      array<Type, Size>&& a) {
	return move(a[Index]);
}

#include "../std/tuple_size.hpp"

template<typename Type, nuint Size>
struct std::tuple_size<array<Type, Size>> {
	static constexpr nuint value = Size;
};

#include "../std/tuple_element.hpp"

template<nuint Index, typename Type, nuint Size>
struct std::tuple_element<Index, array<Type, Size>> {
	using type = Type;
};