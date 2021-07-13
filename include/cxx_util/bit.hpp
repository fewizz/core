#pragma once

#include <cinttypes>
#include <concepts>
#include <iterator>
#include <ranges>
#include <cstdint>
#include <stdint.h>
#include <cstddef>
#include <type_traits>
#include <bit>

#include "object/concepts.hpp"

namespace u {

template<u::trivial T>
class bit_reference {
	using byte_type = std::conditional_t<std::is_const_v<T>, const uint8_t, uint8_t>;

	T& m_o;
	std::size_t m_index;

	auto byte_index() const {
		return std::endian::native != std::endian::little ?
			(sizeof(T) - 1 - (m_index >> 3))
			:
			(m_index >> 3);
	}

	byte_type& byte_ref() const {
		byte_type* bytes = (byte_type*)&m_o;
		return bytes[byte_index()];
	}

public:
	constexpr bit_reference(T& o, std::size_t index) : m_o{ o }, m_index{ index } {}

	auto& operator = (bool b) const {
		set(b);
		return *this;
	}

	operator bool () const {
		return get();
	}

	bool get() const {
		return (
			byte_ref() >> (m_index & 0b111)
		) & 1;
	}

	void set(bool value = 1) const {
		byte_ref() |= value << (m_index & 0b111);
	}

	auto operator <=> (const bit_reference&) const = default;
};

template<u::trivial T>
struct bits_iterator {
	T m_o;
	std::size_t m_bit_index = 0;

	using difference_type = ptrdiff_t;
	using value_type = bit_reference<T>;
	using reference = value_type;

	reference operator * () const {
		return { m_o, m_bit_index };
	}

	auto& operator ++ () {
		++m_bit_index;
		return *this;
	}

	auto& operator -- () {
		--m_bit_index;
		return *this;
	}

	auto operator ++ (int) {
		bits_iterator copy{*this};
		++(*this);
		return copy;
	}

	auto operator -- (int) {
		bits_iterator copy{*this};
		--(*this);
		return copy;
	}

	auto& operator += (std::size_t n) {
		m_bit_index += n; return *this;
	}
	
	auto& operator += (difference_type n) {
		return (*this) += std::size_t(n < 0 ? -n : n);
	}

	auto& operator -= (difference_type n) { return (*this) += -n; }

	auto operator + (difference_type n) const { return bits_iterator{*this} += n; }
	auto operator - (difference_type n) const { return bits_iterator{*this} -= n; }

	friend auto operator + (difference_type n, bits_iterator it) { return it + n; }

	auto operator - (const bits_iterator& other) const {
		return difference_type(m_bit_index - other.m_bit_index);
	}

	reference operator [] (difference_type n) const {
		return *(bits_iterator{ *this } += n);
	}

	auto operator <=> (const bits_iterator&) const = default;
};

template<u::trivial T>
class bits_reference {
	T& m_o;

public:
	using value_type = bit_reference<T>;

	bits_reference(T& o) : m_o{ o } {}

	bits_iterator<T> begin() { return { m_o, 0 }; }
	bits_iterator<const T> begin() const { return { m_o, 0 }; }

	bits_iterator<T> end() { return { m_o, sizeof(T) * 8 }; }
	bits_iterator<const T> end() const { return { m_o, sizeof(T) * 8 }; }

	bit_reference<T> operator [] (std::size_t n) { return { m_o, n }; }
	bit_reference<const T> operator [] (std::size_t n) const { return { m_o, n }; }

	constexpr auto size() const { return sizeof(T) * 8; }
};


}