#pragma once

//#include "character.hpp"
#include <bits/iterator_concepts.h>
#include <bits/stdint-uintn.h>
#include <compare>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include "character.hpp"
#include "encoding/encoding.hpp"

namespace u {

template<
	enc::encoding E,
	class It
>
struct character_view;

template<enc::encoding E, class It>
struct character_iterator {
private:
	It m_begin;
	It m_current;
	It m_end;
public:
	using base_value_type = std::iter_value_t<It>;
	using base_iterator_category =
		typename std::__detail::__iter_concept<It>;
	static constexpr bool base_is_random_access
		= std::is_base_of_v<
			std::random_access_iterator_tag,
			base_iterator_category
		>;
	
	static constexpr bool encoding_is_fixed
		= enc::is_fixed_width_encoding_v<E>;

	using difference_type = std::ptrdiff_t;
	using value_type = character_view<E, It>;

	using iterator_category
		= std::conditional_t<
			encoding_is_fixed && base_is_random_access,
			std::random_access_iterator_tag,
			std::forward_iterator_tag
		>;
	
	static constexpr bool is_random_access
		= std::is_base_of_v<
			std::random_access_iterator_tag,
			iterator_category
		>;

	character_iterator() = default;
	character_iterator(character_iterator&& other) = default;
	character_iterator& operator = (character_iterator&& other) = default;
	character_iterator(const character_iterator& other) = default;
	character_iterator& operator = (const character_iterator& other) = default;

private:
	constexpr void check() const {
		if(m_begin > m_current)
			throw std::out_of_range{ "begin passed current" };
		if(m_current > m_end)
			throw std::out_of_range{ "current passed end" };
	}

	constexpr auto width() const {
		return enc::size<E>(m_current, m_end);
	}
public:
	constexpr character_iterator(
		It begin,
		It cur,
		It end
	) :
	m_begin{ begin }, m_current{ cur }, m_end{ end } {
		check();
	}

	auto base_begin() {
		return m_begin;
	}

	auto base() {
		return m_current;
	}

	auto base_end() {
		return m_end;
	}

	constexpr value_type operator * () const {
		check();
		auto ch_end = m_current;
		std::advance(ch_end, width());
		return { m_current, ch_end };
	}

	constexpr auto& operator ++ () {
		check();
		m_current += width();
		return *this;
	}

	constexpr auto operator ++ (int) {
		auto copy = *this;
		++(*this);
		return copy;
	}

	character_iterator& operator -- () const;
	character_iterator operator -- (int) const;

	difference_type
	operator - (character_iterator that) const
	requires( is_random_access ) {
		return (m_current - that.m_current);
	}

	auto& operator += (difference_type n)
	requires( is_random_access ) {
		m_current += n;
		return *this;
	}

	auto& operator -= (difference_type n)
	requires( is_random_access ) {
		m_current -= n;
		return *this;
	}

	auto operator + (difference_type n) const
	requires( is_random_access ) {
		auto copy = *this;
		return copy += n;
	}

	auto operator - (difference_type n) const
	requires( is_random_access ) {
		auto copy = *this;
		return copy -= n;
	}

	constexpr void skip (
		typename std::iterator_traits<It>::difference_type n
	) {
		std::advance(m_current, n);
	}

	constexpr std::strong_ordering
	operator <=> (const character_iterator& other) const {
		return m_current <=> other.m_current;
	}

	constexpr bool
	operator == (const character_iterator& other) const = default;

	value_type operator [] (difference_type n) const {
		auto copy = *this;
		copy += n;
		return *copy;
	}
};

template<enc::encoding E, class It>
character_iterator<E, It>
operator + (
	typename character_iterator<E, It>::difference_type n,
	character_iterator<E, It> it
) {
	return it + n;
}

template<enc::encoding E, class It>
character_iterator<E, It> make_character_iterator(It b, It e) {
	return { b, b, e };
}

template<enc::encoding E, class It>
character_iterator<E, It> make_character_iterator_end(It b, It e) {
	return { b, e, e };
}

template<enc::encoding E, class I>
character_iterator<E, I> begin(character_iterator<E, I> ci) {
	return ci;
}

template<enc::encoding E, class I>
character_iterator<E, I> end(character_iterator<E, I> ci) {
	return make_character_iterator_end<E, I>(ci.base_begin(), ci.base_end());
}

}