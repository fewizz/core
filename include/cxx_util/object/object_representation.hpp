#pragma once

#include <array>
#include <span>
#include <bit>
#include <ranges>
#include <iterator>

#include "concepts.hpp"
#include "operations.hpp"

namespace u {

template<u::trivial T, std::endian E = std::endian::native>
struct object_representation_copy : std::array<std::byte, sizeof(T)> {
	using base_type = std::array<std::byte, sizeof(T)>;

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
	std::byte* m_ptr;

	object_representation_reference(const T& obj)
		: m_ptr{ (std::byte*)&obj } {}

	object_representation_reference(object_representation_reference&&) = default;

	using iterator = std::conditional_t<
			E == std::endian::native,
			std::byte*,
			std::reverse_iterator<std::byte*>
		>;

	iterator begin() const {
		return { E == std::endian::native ? m_ptr : m_ptr + sizeof(T) - 1 };
	}

	iterator end() const {
		return { E == std::endian::native ? m_ptr + sizeof(T) : m_ptr - 1 };
	}

	std::byte& front() const { return *begin(); }
	std::byte& back() const { return *(end() - 1); }
	
	auto size() const { return sizeof(T); }

	T create() const {
		return u::object_from_bytes<T>(begin());
	}
};

}

template <u::trivial T, std::endian E>
inline constexpr bool std::ranges::enable_borrowed_range<u::object_representation_reference<T, E>> = true;