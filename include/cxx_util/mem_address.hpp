#pragma once

#include <compare>
#include <iterator>
#include <type_traits>
#include "interface/iterator.hpp"

namespace u {

struct mem_address : u::contiguous_iterator<mem_address, u::value_type<std::byte>>{
private:
	using base_type = contiguous_iterator;

	std::byte* m_memory_address;

public:
	mem_address() = default;
	mem_address(mem_address&& that) = default;
	mem_address& operator = (mem_address&& that) = default;
	mem_address(const mem_address& that) = default;
	mem_address& operator = (const mem_address& that) = default;

	template<class T>
	mem_address(T* ptr)
	: m_memory_address {
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

	difference_type operator - (mem_address that) const {
		return m_memory_address - that.m_memory_address;
	}

	using base_type::operator -;

	std::strong_ordering
	operator <=> (const mem_address& that) const {
		return m_memory_address <=> that.m_memory_address;
	}

	bool operator == (const mem_address& that) const {
		return *this <=> that == 0;
	};

	std::strong_ordering
	operator <=> (auto* ptr) const {
		return *this <=> mem_address{ ptr };
	}

	bool operator == (auto* ptr) const {
		return *this <=> ptr == 0;
	};
};

inline std::strong_ordering
operator <=> (auto* ptr, mem_address loc) {
	return mem_address{ ptr } <=> loc;
}

}