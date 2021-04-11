#pragma once

#include <bit>
#include <compare>
#include <iterator>
#include <memory>
#include "byte.hpp"
#include "mem_address.hpp"
#include "object.hpp"
#include "math.hpp"
#include "interface/iterator.hpp"

namespace u {

template<class It, std::endian E = std::endian::native>
class byte_iterator
: public u::iterator<
	typename std::__detail::__iter_concept<It>,
	byte_iterator<It>,
	u::value_type<std::byte>
> {
	It m_it;
	std::size_t m_byte_index = 0;

	using base_type = u::iterator<
		typename std::__detail::__iter_concept<It>,
		byte_iterator<It>,
		u::value_type<std::byte>
	>;

public:
	using base_value_type = std::iter_value_t<It>;

	static constexpr std::size_t
		base_value_type_size = sizeof(base_value_type);

	using base_iterator_category =
		typename std::__detail::__iter_concept<It>;

	using typename base_type::difference_type;

	byte_iterator() = default;

	byte_iterator(It it)
	: m_it{ it } {}

	auto byte_index() const {
		return m_byte_index;
	}

	std::byte& operator * () const {
		auto real_index = m_byte_index;

		if constexpr (E == std::endian::big)
			real_index
				= base_value_type_size - real_index - 1;

		return u::obj_representation_reference{ *m_it }[real_index];
	}

	byte_iterator& operator ++ () {
		++m_byte_index;
		if(m_byte_index == base_value_type_size) {
			++m_it;
			m_byte_index = 0;
		}
		return *this;
	}
	using base_type::operator ++ ;

	byte_iterator& operator -- ()
	requires(std::bidirectional_iterator<It>) {
		if(m_byte_index == 0) {
			--m_it;
			m_byte_index = base_value_type_size;
		}
		--m_byte_index;

		return *this;
	}

	byte_iterator operator -- (int)
	requires(std::bidirectional_iterator<It>) {
		return base_type::operator -- (int());
	}

	byte_iterator& operator += (difference_type n) {
		static_assert(std::random_access_iterator<It>);
		auto full = n + m_byte_index;

		auto df = u::div_floor(
			full,
			base_value_type_size
		);

		m_it += df.quot;
		m_byte_index = df.rem;

		return *this;
	}

	difference_type operator - (const byte_iterator& that) const {
		return
			(m_it - that.m_it)
			* base_value_type_size
			+ (byte_index() - that.byte_index())
		;
	}

	auto& operator [] (difference_type n) const {
		static_assert(std::random_access_iterator<It>);
		return *(*this + n);
	}

	std::strong_ordering
	operator <=> (const byte_iterator& that) const {
		if(auto res = std::compare_three_way{}(m_it, that.m_it); res != 0) {
			return res;
		}

		return m_byte_index <=> that.m_byte_index;
	};

	bool operator == (const byte_iterator& that) const {
		return *this <=> that == 0;
	}
};

template<std::endian Endian, class It>
inline auto make_byte_iterator(It it) {
	return byte_iterator<It, Endian>{ it };
}

}