#pragma once

#include "integer.hpp"
#include "meta/type/remove_reference.hpp"
#include "meta/types/are_same.hpp"
#include "meta/types/first.hpp"

template<typename Type, nuint Size>
struct array {
	using value_type = Type;
	value_type m_array[Size];

	constexpr nuint size() const {
		return Size;
	}

	constexpr auto begin() const {
		return m_array;
	}

	constexpr auto begin() {
		return m_array;
	}

	constexpr auto end() const {
		return m_array + Size;
	}

	constexpr auto end() {
		return m_array + Size;
	}

	constexpr auto& operator [] (nuint index) {
		return data()[index];
	}

	constexpr const auto& operator [] (nuint index) const {
		return data()[index];
	}

	constexpr value_type* data() {
		return m_array;
	}

	constexpr const value_type* data() const {
		return m_array;
	}
};

template<typename... Types>
requires(sizeof...(Types) == 1 || types::are_same::for_types<Types...>)
array(Types&&...)
	-> array<
		remove_reference<typename types::first::for_types<Types...>>,
		sizeof...(Types)
	>;