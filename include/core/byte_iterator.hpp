#pragma once

#include <bit>
#include <compare>
#include <cstdint>
#include <iterator>
#include <memory>
#include <type_traits>

#include "iterator/iterator.hpp"
#include "math.hpp"

namespace u {

template<typename It, std::endian E = std::endian::native>
class byte_iterator {
	It m_it;
	std::uint8_t m_index;

public:
	byte_iterator() = default;
	byte_iterator(It it) : m_it{ it }, m_index{ 0 } {}
	byte_iterator(const byte_iterator&) = default;
	byte_iterator& operator = (const byte_iterator&) = default;
	byte_iterator(byte_iterator&&) = default;
	byte_iterator& operator = (byte_iterator&&) = default;

	using base_value_type = std::iter_value_t<It>;

	using difference_type = std::iter_difference_t<It>;
	using element_type = std::conditional_t<std::is_const_v<base_value_type>, const std::byte, std::byte>;

	using reference =
		std::conditional_t<
			std::is_lvalue_reference_v<std::iter_reference_t<It>>,
			element_type&,
			element_type
		>;
	
	// can't be contiguous when iterating in reverse order
	using iterator_concept = std::conditional_t<
			std::is_same_v<u::iter_concept<It>, std::contiguous_iterator_tag>
			&& E != std::endian::native,
			std::random_access_iterator_tag,
			u::iter_concept<It>
		>;
	
	using iterator_category =
		typename std::iterator_traits<It>::iterator_category;

	auto byte_index() const {
		return m_index;
	}

	reference operator * () const {
		return *(this->operator->());
	}

	byte_iterator& operator ++ () {
		if(m_index + 1 == sizeof(base_value_type)) {
			++m_it;
			m_index = 0;
		}
		else {
			++m_index;
		}
		return *this;
	}

	byte_iterator& operator -- () {
		if(m_index == 0) {
			--m_it;
			m_index = sizeof(base_value_type) - 1;
		}
		else {
			--m_index;
		}
		return *this;
	}

	byte_iterator operator ++ (int) {
		byte_iterator copy{ *this };
		++*this;
		return copy;
	}

	byte_iterator operator -- (int) {
		byte_iterator copy{ *this };
		--*this;
		return copy;
	}

	byte_iterator& operator += (difference_type n) {
		auto d = m_index + n;
		auto div_res = u::div_floor(d, sizeof(base_value_type));
		m_it += div_res.quot;
		m_index = div_res.rem;
		return *this;
	}

	byte_iterator& operator -= (difference_type n) {
		return *this += -n;
	}

	byte_iterator operator + (difference_type n) const {
		return byte_iterator { *this } += n;
	}

	byte_iterator operator - (difference_type n) const {
		return byte_iterator { *this } -= n;
	}

	friend byte_iterator operator + (difference_type n, const byte_iterator& it) {
		return it + n;
	}

	difference_type operator - (const byte_iterator& other) const {
		return
			sizeof(base_value_type) * (m_it - other.m_it)
			+
			(m_index - other.m_index);
	}

	reference operator [] (difference_type n) const {
		return *(byte_iterator{ *this } += n);
	}

	element_type* operator -> () const {
		return ((std::byte*)std::to_address(m_it))
		+
		(
			E == std::endian::native ?
			m_index
			:
			sizeof(base_value_type) - 1 - m_index
		);
	}

	auto operator <=> (const byte_iterator& other) const = default;
};

}