#pragma once

#include "integer.hpp"
#include "meta/type/is_reference.hpp"
#include "meta/type/remove_reference.hpp"

template<typename ValueType>
struct span {
	using value_type = ValueType;

	ValueType* m_values;
	nuint m_size;

	constexpr span() = default;

	constexpr span(ValueType* values, nuint size)
		: m_values{ values }, m_size{ size }
	{}

	constexpr span(nuint size, ValueType* values)
		: m_values{ values }, m_size{ size }
	{}

	constexpr nuint size() const {
		return m_size;
	}

	constexpr const value_type* begin() const {
		return m_values;
	}

	constexpr value_type* begin() {
		return m_values;
	}

	constexpr const value_type* end() const {
		return m_values + m_size;
	}

	constexpr value_type* end() {
		return m_values + m_size;
	}

	constexpr auto& operator [] (nuint index) {
		return data()[index];
	}

	constexpr const auto& operator [] (nuint index) const {
		return data()[index];
	}

	constexpr auto& data() {
		return m_values;
	}

	constexpr auto& data() const {
		return m_values;
	}

	template<typename Type>
	constexpr span<Type> cast() {
		return span<Type>{ (Type*) data(), size() };
	}

};

template<typename ValueType>
requires(type::is_reference::for_type<ValueType>)
struct span<ValueType> {
	using clear_value_type = remove_reference<ValueType>;
	using value_type = clear_value_type&;

	clear_value_type** m_values;
	nuint m_size;
	
	struct iterator {
		clear_value_type** ptr;

		iterator(clear_value_type** ptr) : ptr{ ptr } {}

		clear_value_type& operator * () const {
			return **ptr;
		}

		auto& operator ++ () {
			++ptr;
			return *this;
		}

		auto& operator += (nuint n) {
			ptr += n;
			return *this;
		}
	
		auto operator + (nuint n) const {
			return iterator{ *this } += n;
		}
	
		bool operator == (const iterator other) const {
			return ptr == other.ptr;
		}
	};

	constexpr span(clear_value_type** values, nuint size)
		: m_values{ values }, m_size{ size }
	{}

	constexpr span(nuint size, clear_value_type** values)
		: m_values{ values }, m_size{ size }
	{}

	constexpr nuint size() const {
		return m_size;
	}

	constexpr iterator begin() const {
		return { m_values };
	}

	constexpr iterator end() const {
		return { m_values + m_size };
	}

	constexpr clear_value_type** data() const {
		return m_values;
	}

	constexpr clear_value_type& operator [] (nuint index) {
		return *(begin() + index);
	}

	constexpr const clear_value_type& operator [] (nuint index) const {
		return *(begin() + index);
	}

};