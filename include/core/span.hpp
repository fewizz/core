#pragma once

#include "integer.hpp"
#include "type/is_reference.hpp"
#include "type/remove_reference.hpp"
//#include "wrapper/of_integer.hpp"
//#include "wrapper/of_pointer_to.hpp"

//struct size : wrapper::of_integer<uint> {};

//template<typename T>
//struct beginning : wrapper::of_pointer_to<T> {};

template<typename ValueType>
struct span {
	using value_type = ValueType;

	ValueType* m_values;
	uint m_size;

	span(ValueType* values, uint size)
		: m_values{ values }, m_size{ size }
	{}

	span(uint size, ValueType* values)
		: m_values{ values }, m_size{ size }
	{}

	constexpr uint size() const {
		return m_size;
	}

	constexpr auto begin() const {
		return m_values;
	}

	constexpr auto end() const {
		return m_values + m_size;
	}

	constexpr auto& operator [] (uint index) {
		return data()[index];
	}

	constexpr const auto& operator [] (uint index) const {
		return data()[index];
	}

	constexpr value_type* data() {
		return m_values;
	}

	constexpr const value_type* data() const {
		return m_values;
	}

};

template<typename ValueType>
requires(type::is_reference::for_type_of<ValueType>)
struct span<ValueType> {
	using clear_value_type = type::remove_reference::for_type_of<ValueType>;
	using value_type = clear_value_type&;

	clear_value_type** m_values;
	uint m_size;
	
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

		auto& operator += (uint n) {
			ptr += n;
			return *this;
		}
	
		auto operator + (uint n) const {
			return iterator{ *this } += n;
		}
	
		bool operator == (const iterator other) const {
			return ptr == other.ptr;
		}
	};

	span(clear_value_type** values, uint size)
		: m_values{ values }, m_size{ size }
	{}

	span(uint size, clear_value_type** values)
		: m_values{ values }, m_size{ size }
	{}

	constexpr uint size() const {
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

	//constexpr const clear_value_type** data() const {
	//	return m_values;
	//}

	constexpr clear_value_type& operator [] (uint index) {
		return *(begin() + index);
		//return *it;
	}

	constexpr const clear_value_type& operator [] (uint index) const {
		return *(begin() + index);
	}
};