#pragma once

#include <compare>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include "codepoint.hpp"
#include "interface/iterator.hpp"
#include "codec.hpp"
#include "character.hpp"

namespace u {

template <
	u::codec C,
	class It
>
struct character_view;

template<u::codec C, class It>
struct encoded_string_iterator
: u::input_iterator <
	encoded_string_iterator<C, It>,
	u::value_type<u::encoded_character_view<C, It>>,
	u::difference_type<std::iter_difference_t<It>>
> {
private:
	It m_it;
public:
	using base_type = u::input_iterator <
		encoded_string_iterator<C, It>,
		u::value_type<u::encoded_character_view<C, It>>,
		u::difference_type<std::iter_difference_t<It>>
	>;

	using typename base_type::value_type;
	using typename base_type::iterator_category;

	encoded_string_iterator() = default;
	encoded_string_iterator(encoded_string_iterator&& other) = default;
	encoded_string_iterator& operator = (encoded_string_iterator&& other) = default;
	encoded_string_iterator(const encoded_string_iterator& other) = default;
	encoded_string_iterator& operator = (const encoded_string_iterator& other) = default;

public:
	constexpr encoded_string_iterator(It it)
	: m_it{ it } {}

	auto base() {
		return m_it;
	}

	u::encoded_character_view<C, It> operator * () const {
		return { m_it, m_it + typename C::decoder_type{}.size(m_it)};
	}

	auto& operator ++ () {
		m_it += typename C::decoder_type{}.size(m_it);
		return *this;
	}

	using base_type::operator ++ ;

	std::strong_ordering
	operator <=> (const encoded_string_iterator& other) const {
		return m_it <=> other.m_it;
	}

	bool operator == (const encoded_string_iterator& other) const {
		return (m_it <=> other) == 0;
	}
};

/*template<enc::encoding E, class It>
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
}*/

}