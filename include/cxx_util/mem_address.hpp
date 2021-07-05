#pragma once

#include <compare>
#include <iterator>
#include <type_traits>
#include "interface/iterator.hpp"
#include "object/concepts.hpp"

namespace u {

struct mem_address : u::i::contiguous_iterator<mem_address, u::i::value_type<std::byte>>{
private:
	using base_type = contiguous_iterator;

	std::byte* m_memory_address;

public:
	constexpr mem_address() = default;
	constexpr mem_address(mem_address&& that) = default;
	constexpr mem_address& operator = (mem_address&& that) = default;
	constexpr mem_address(const mem_address& that) = default;
	constexpr mem_address& operator = (const mem_address& that) = default;

	template<u::c::object O>
	constexpr mem_address(O* ptr)
	: m_memory_address { std::bit_cast<std::byte*>(ptr) } {}

	constexpr element_type& operator * () const {
		return *m_memory_address;
	}

	constexpr mem_address& operator += (difference_type n) {
		m_memory_address += n;
		return *this;
	}

	constexpr difference_type operator - (mem_address that) const {
		return m_memory_address - that.m_memory_address;
	}

	using base_type::operator -;

	std::strong_ordering
	constexpr operator <=> (const mem_address& that) const {
		return m_memory_address <=> that.m_memory_address;
	}

	constexpr bool operator == (const mem_address& that) const {
		return *this <=> that == 0;
	};

	std::strong_ordering
	constexpr operator <=> (auto* ptr) const {
		return *this <=> mem_address{ ptr };
	}

	constexpr bool operator == (auto* ptr) const {
		return *this <=> ptr == 0;
	};
};

inline std::strong_ordering
constexpr operator <=> (auto* ptr, mem_address loc) {
	return mem_address{ ptr } <=> loc;
}

}