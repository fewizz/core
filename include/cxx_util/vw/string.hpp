#pragma once

#include <algorithm>
#include <bits/c++config.h>
#include <compare>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include "../encoding/encoding.hpp"
#include "character_iterator.hpp"
#include "string_view.hpp"
#include "common.hpp"

namespace vw {

template<
	enc::encoding E,
	class T,
	class Allocator = std::allocator<T>
>
struct basic_string : vw::internal::string_common<basic_string<E, T, Allocator>, E, T> {
	using common_type = vw::internal::string_common<basic_string<E, T, Allocator>, E, T>;
	using typename common_type::value_type;

	using allocator_type = Allocator;
	using typename common_type::size_type;

	using common_type::npos;

private:
	Allocator m_allocator;
	size_type m_size = 0;
	T* m_data = nullptr;

public:
	constexpr explicit basic_string(const Allocator& a = Allocator{} )
		: m_allocator{ a } {}

	constexpr basic_string(const basic_string&) = default;
	constexpr basic_string(basic_string&&) = default;

	constexpr basic_string(const T* p, const Allocator& a = Allocator{} )
		: basic_string(p, p + std::basic_string_view<T>{p}.size(), a) {}

	template<std::input_iterator It>
	constexpr basic_string( It b, It e, const Allocator& a = Allocator() )
		:
		m_allocator(a),
		m_size { size_type(std::distance(b, e)) },
		m_data { m_allocator.allocate(m_size)/*new T[ m_size ]*/ }
	{
		size_type index = 0;
		while(b < e) {
			std::allocator_traits<allocator_type>::construct(
				m_allocator,
				m_data + (index++),
				*(b++)
			);
		}
	}

	constexpr ~basic_string() {
		if(m_data) {
			m_allocator.deallocate(m_data, m_size);
		}
	}

	constexpr T* data() const {
		return m_data;
	}

	constexpr size_type raw_size() const {
		return m_size;
	}

	constexpr size_type capacity() const {
		return common_type::length();
	}

	constexpr void clear() {
		m_allocator.deallocate(m_data, m_size);
		m_size = 0;
		m_data = nullptr;
	}

	constexpr operator basic_string_view<E, const T> () const {
		return { data(), raw_size() };
	}

};

using utf8_string = basic_string<enc::utf8, char8_t>;

}