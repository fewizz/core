#pragma once

#include "./integer.hpp"
#include "./type.hpp"
#include "./values/of.hpp"
#include "./types.hpp"

template<typename Type, nuint Size>
struct array {
	Type array_[Size];

	constexpr nuint size() const {
		return Size;
	}

	constexpr const Type* iterator() const & { return array_; }
	constexpr       Type* iterator()       & { return array_; }

	constexpr const Type* sentinel() const & { return array_ + Size; }
	constexpr       Type* sentinel()       & { return array_ + Size; }

	constexpr const Type* elements_ptr() const & { return array_; }
	constexpr       Type* elements_ptr()       & { return array_; }

	constexpr decltype(auto) operator [] (nuint index) const {
		return array_[index];
	}
	constexpr decltype(auto) operator [] (nuint index) {
		return array_[index];
	}

	template<typename F>
	constexpr decltype(auto) pass(F&& f) const {
		return [&]<nuint... Indices>(indices::of<Indices...>)
		-> decltype(auto) {
			return f((*this)[Indices]...);
		}(indices::from<0>::to<Size>{});
	}
};

template<typename... Types>
requires(sizeof...(Types) == 1 || types_are_same<Types...>)
array(Types&&...)
	-> array<
		__type::remove_reference<first_type<Types...>>,
		sizeof...(Types)
	>;

template<nuint Index, typename Type, nuint Size>
constexpr const Type&  get(const array<Type, Size>&  a) { return a[Index]; }

template<nuint Index, typename Type, nuint Size>
constexpr       Type&  get(      array<Type, Size>&  a) { return a[Index]; }

template<nuint Index, typename Type, nuint Size>
constexpr const Type&& get(const array<Type, Size>&& a) { return a[Index]; }

template<nuint Index, typename Type, nuint Size>
constexpr       Type&& get(      array<Type, Size>&& a) { return a[Index]; }

#include <std/tuple_size.hpp>

template<typename Type, nuint Size>
struct std::tuple_size<array<Type, Size>> {
	static constexpr nuint value = Size;
};

#include <std/tuple_element.hpp>

template<nuint Index, typename Type, nuint Size>
struct std::tuple_element<Index, array<Type, Size>> {
	using type = Type;
};