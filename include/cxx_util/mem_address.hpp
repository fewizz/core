#pragma once

#include <compare>
#include <iterator>
#include <type_traits>
//#include "memory_location.hpp"

namespace u {

class mem_address {
	std::byte* m_memory_address;

public:
	using element_type = std::byte;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::contiguous_iterator_tag;

	mem_address() = default;
	mem_address(mem_address&& that) = default;
	mem_address& operator = (mem_address&& that) = default;
	mem_address(const mem_address& that) = default;
	mem_address& operator = (const mem_address& that) = default;

	template<class T>
	mem_address(T* ptr)
	: m_memory_address{
		reinterpret_cast<std::byte*>(
			const_cast< std::remove_const_t<T>* >(ptr)
		)
	} {}

	element_type& operator * () const {
		return *m_memory_address;
	}

	mem_address& operator += (difference_type n) {
		m_memory_address += n;
		return *this;
	}

	mem_address& operator -= (difference_type n) {
		return *this += -n;
	}

	mem_address& operator ++ () {
		return *this += 1;
	}

	mem_address& operator -- () {
		return *this -= 1;
	}

	mem_address operator + (difference_type n) const {
		return { m_memory_address + n};
	}

	mem_address operator - (difference_type n) const {
		return *this + (-n);
	}

	mem_address operator ++ (int) {
		auto prev = *this;
		++(*this);
		return prev;
	}

	mem_address operator -- (int) {
		auto prev = *this;
		--(*this);
		return prev;
	}

	difference_type operator - (mem_address that) const {
		return m_memory_address - that.m_memory_address;
	}

	element_type& operator [] (difference_type n) const {
		return m_memory_address[n];
	}

	element_type* operator -> () const {
		return m_memory_address;
	}

	std::strong_ordering
	operator <=> (const mem_address& that) const = default;

	bool operator == (const mem_address& that) const = default;

	std::strong_ordering
	operator <=> (auto* ptr) const {
		return *this <=> mem_address{ ptr };
	}

	bool operator == (auto* ptr) const {
		return *this <=> ptr == 0;
	};
};

inline mem_address
operator + (
	typename mem_address::difference_type n,
	mem_address it
) {
	return it + n;
}

inline std::strong_ordering
operator <=> (auto* ptr, mem_address loc) {
	return mem_address{ ptr } <=> loc;
}

}