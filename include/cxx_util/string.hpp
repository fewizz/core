#pragma once

#include <algorithm>
#include <bits/iterator_concepts.h>
#include <compare>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include "byte_range.hpp"
#include "encoding/encoding.hpp"
#include "character_iterator.hpp"
#include "encoding/unicode.hpp"
#include "string_view.hpp"
#include "string_common_base.hpp"
#include "byte_range.hpp"

namespace u {

template<
	enc::encoding E,
	class T = std::byte,
	class A = std::allocator<T>
>
struct basic_string
: internal::string_common_base<
	basic_string<E, T, A>,
	E,
	T*
> {
	using base_type = internal::string_common_base<
		basic_string<E, T, A>,
		E,
		T*
	>;

	using allocator_type = A;
	using typename base_type::size_type;
	using typename base_type::value_type;
	using typename base_type::iterator;
	using base_type::npos;

private:
	A m_allocator;
	size_type m_size = 0;
	T* m_data = nullptr;

public:
	T* raw_begin() const {
		return m_data;
	}

	T* raw_end() const {
		return m_data + m_size;
	}

	constexpr explicit basic_string(const A& a = A{} )
		: m_allocator{ a } {}

	constexpr basic_string(const basic_string& that) {
		m_allocator = that.m_allocator;
		m_size = that.m_size;
		m_data = m_allocator.allocate(m_size);
		u::byte_range<decltype(m_data)>{
			that.raw_begin(),
			that.raw_end()
		}.copy_to(m_data);
	}
	basic_string& operator = (const basic_string& that) {
		clear();
		m_allocator = that.m_allocator;
		m_size = that.m_size;
		m_data = m_allocator.allocate(m_size);
		u::byte_range<decltype(m_data)>{
			that.raw_begin(),
			that.raw_end()
		}.copy_to(m_data);
		return *this;
	}

	basic_string(basic_string&& that) {
		m_allocator = that.m_allocator;
		m_data = std::exchange(that.m_data, nullptr);
		m_size = std::exchange(that.m_size, 0);
	};

	constexpr basic_string(const T* p, const A& a = A{} )
	: basic_string(p, p + std::basic_string_view<T>{ p }.size(), a) {}

	template<std::input_iterator It>
	requires( not std::ranges::range<std::iter_value_t<It>> )
	constexpr basic_string(It b, It e, const A& a = A())
	:
		m_allocator{ a },
		m_size { size_type(std::distance(b, e)) },
		m_data { m_allocator.allocate(m_size) }
	{
		u::byte_range<It>{ b, e }.copy_to(m_data);
	}

	template<std::ranges::range R>
	constexpr basic_string(const R& range)
		: basic_string(
			std::ranges::begin(range),
			std::ranges::end(range)
		) {}

	constexpr ~basic_string() {
		clear();
	}

	constexpr T* data() const {
		return m_data;
	}

	constexpr void clear() {
		if(m_data) {
			m_allocator.deallocate(m_data, m_size);
			m_size = 0;
			m_data = nullptr;
		}
	}

	constexpr operator u::basic_string_view<E, const T*> () const {
		return { raw_begin(), raw_end() };
	}
};

using ascii_string = basic_string<enc::ascii>;
using utf8_string = basic_string<enc::utf8, char8_t>;
using u8_string = utf8_string;
using uncode_string = basic_string<enc::char_set_encoding<enc::unicode>>;

}