#pragma once

#include <array>
#include <bit>
#include <ranges>
#include <iterator>

#include "concepts.hpp"
#include "operations.hpp"

namespace u { namespace obj {

template<u::trivial T, std::endian E = std::endian::native>
struct bytes
: std::array<std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>, sizeof(T)> {
	using base_type
		= std::array<std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>, sizeof(T)>;

	bytes(const T& obj) {
		u::obj::write<T, E>(obj, base_type::begin());
	}

	T create() const {
		return u::obj::read<T, E>(base_type::begin());
	}
};

// TODO inherit from view_interface when libc++'s ranges done or clang fixed
template<u::trivial T, std::endian E = std::endian::native>
struct bytes_view : std::ranges::view_base {
	using value_type = std::conditional_t<std::is_const_v<T>, const std::byte, std::byte>;

	value_type* m_ptr;

	bytes_view(T& obj)
		: m_ptr{ (value_type*)&obj } {}

	bytes_view(bytes_view&&) = default;

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
		return u::obj::read<T>(begin());
	}
};

}}

template <u::trivial T, std::endian E>
inline constexpr bool std::ranges::enable_borrowed_range<u::obj::bytes_view<T, E>> = true;