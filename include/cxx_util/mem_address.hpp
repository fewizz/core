#pragma once

#include <compare>
#include <iterator>
#include <type_traits>
#include "interface/iterator.hpp"

namespace u {

struct mem_address
:
	u::incr_and_decr_from_add_and_sub_assign<mem_address>,
	u::sub_assign_from_add_assign<mem_address>,
	u::add_from_add_assign<mem_address>,
	u::sub_from_add_assign<mem_address>,
	u::subscipt_from_add_and_dereference<mem_address>,
	u::member_pointer_from_dereference<mem_address, std::byte>
{
private:
	std::byte* m_memory_address;

public:
	using incr_and_decr_from_add_and_sub_assign::operator ++;
	using incr_and_decr_from_add_and_sub_assign::operator --;
	using sub_from_add_assign::operator -;

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

	difference_type operator - (mem_address that) const {
		return m_memory_address - that.m_memory_address;
	}

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