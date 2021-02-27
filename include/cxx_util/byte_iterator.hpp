#pragma once

#include <bit>
#include <compare>
#include <iterator>
#include <memory>
#include "byte.hpp"
#include "mem_address.hpp"
#include "obj_representation.hpp"
#include "math.hpp"

namespace u {

template<class It, std::endian E = std::endian::native>
class byte_iterator {
	It m_it;
	std::size_t m_byte_index = 0;

public:
	using base_value_type = std::iter_value_t<It>;

	static constexpr std::size_t
		base_value_type_size = sizeof(base_value_type);
	
	using element_type = std::byte;
	using difference_type = std::ptrdiff_t;

	using base_iterator_category =
		typename std::__detail::__iter_concept<It>;

	using iterator_category = base_iterator_category;

	byte_iterator() = default;

	byte_iterator(It it)
	: m_it{ it } {}

	element_type& operator * () const {
		return u::obj_representation{ *m_it }[m_byte_index];
	}

	element_type* operator -> () const {
		return & u::obj_representation{ *m_it }[m_byte_index];
	}

	byte_iterator& operator ++ () {
		++m_byte_index;
		if(m_byte_index == base_value_type_size) {
			++m_it;
			m_byte_index = 0;
		}
		return *this;
	}

	byte_iterator& operator -- () {
		if(m_byte_index == 0) {
			--m_it;
			m_byte_index = base_value_type_size;
		}
		--m_byte_index;

		return *this;
	}

	byte_iterator operator ++ (int) {
		auto prev = *this;
		++*this;
		return prev;
	}

	byte_iterator operator -- (int) {
		auto prev = *this;
		--*this;
		return prev;
	}

	byte_iterator& operator += (difference_type n) {
		auto full = n + m_byte_index;

		auto df = u::div_floor(
			full,
			base_value_type_size
		);

		m_it += df.quot;
		m_byte_index = df.rem;

		return *this;
	}

	byte_iterator& operator -= (difference_type n) {
		return *this += (-n);
	}

	byte_iterator operator + (difference_type n) const {
		static_assert(std::random_access_iterator<It>);
		auto copy = *this;
		copy += n;
		return copy;
	}

	byte_iterator operator - (difference_type n) const {
		return *this + (-n);
	}

	difference_type operator - (byte_iterator that) const {
		return 
			u::mem_address{ std::to_address(*this) }
			-
			u::mem_address{ std::to_address(*that) };
	}

	auto& operator [] (difference_type n) const {
		return *(*this + n);
	}

	std::strong_ordering
	operator <=> (const byte_iterator& that) const = default;
};

//template<std::endian E>
//byte_iterator(auto&& it) -> byte_iterator<decltype(it), E>;

template<std::endian E, class It>
inline auto operator - (
	typename byte_iterator<It, E>::difference_type n,
	byte_iterator<It, E> it
) {
	return it - (-n);
}

template<std::endian E, class It>
inline auto operator + (
	typename byte_iterator<It, E>::difference_type n,
	byte_iterator<It, E> it
) {
	return it + n;
}

template<std::endian Endian, class It>
inline auto make_byte_iterator(It it) {
	return byte_iterator<It, Endian>{ it };
}

}