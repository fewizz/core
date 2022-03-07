#pragma once

#include "span.hpp"

#include "range/of_value_type.hpp"

template<typename T>
struct box {
	using value_type = T;

	span<T> m_storage;
	nuint m_size = 0;

	constexpr nuint size() const {
		return m_size;
	}

	constexpr T* begin() {
		return m_storage.begin();
	}

	constexpr const T* begin() const {
		return m_storage.begin();
	}

	constexpr T* end() {
		return m_storage.begin() + m_size;
	}

	constexpr const T* end() const {
		return m_storage.begin() + m_size;
	}

	constexpr void push_back(T value) {
		m_storage[size()] = move(value);
		m_size++;
	}

	constexpr void push_back(range::of_value_type<T> auto range) {
		for(auto& value : range) {
			push_back(value);
		}
	}

	constexpr auto data() const {
		return m_storage.data();
	}

	constexpr auto data() {
		return m_storage.data();
	}
};

template<typename T, typename F>
// todo rename
decltype(auto) view_box_of_capacity(nuint capacity, F&& f) {
	T storage[capacity];

	box<T> b{ { storage, capacity }, 0 };

	return f(b);
}