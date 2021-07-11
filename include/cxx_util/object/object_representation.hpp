#pragma once

#include <array>
#include <bit>
#include <ranges>
#include <iterator>

#include "concepts.hpp"
#include "operations.hpp"

namespace u {

template<u::trivial T, std::endian E = std::endian::native>
struct object_representation_copy
: std::array<std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>, sizeof(T)> {
	using base_type
		= std::array<std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>, sizeof(T)>;

	object_representation_copy(const T& obj) {
		u::object_to_bytes<T, E>(obj, base_type::begin());
	}

	T create() const {
		return u::object_from_bytes<T, E>(base_type::begin());
	}
};

// TODO inherit from view_interface when libc++'s ranges done or clang fixed
template<u::trivial T, std::endian E = std::endian::native>
struct object_representation_reference : std::ranges::view_base {
	using value_type = std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>;

	value_type* m_ptr;

	object_representation_reference(T& obj)
		: m_ptr{ (value_type*)&obj } {}

	object_representation_reference(object_representation_reference&&) = default;

	template<bool Const>
	using iterator = std::conditional_t<
			E == std::endian::native,
			std::conditional_t<Const, const value_type*, value_type*>,
			std::reverse_iterator<std::conditional_t<Const, const value_type*, value_type*>>
		>;

private:
	auto raw_begin() const {
		return E == std::endian::native ? m_ptr : m_ptr + sizeof(T) - 1;
	}

	auto raw_end() const {
		return E == std::endian::native ? m_ptr + sizeof(T) : m_ptr - 1;
	}
public:

	iterator<false> begin() { return { raw_begin() }; }
	iterator<true> begin() const { return { raw_begin() }; }

	iterator<false> end() { return { raw_end() }; }
	iterator<true> end() const { return end(); }

	value_type& front() { return *begin(); }
	const value_type& front() const { return *begin(); }

	value_type& back() { return *(end() - 1); }
	const value_type& back() const { return *(end() - 1); }
	
	auto size() const { return sizeof(T); }

	T create() const {
		return u::object_from_bytes<T>(begin());
	}
};

}

template <u::trivial T, std::endian E>
inline constexpr bool std::ranges::enable_borrowed_range<u::object_representation_reference<T, E>> = true;