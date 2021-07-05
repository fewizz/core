#pragma once

#include <compare>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include "codepoint.hpp"
#include "interface/iterator.hpp"
#include "codec.hpp"
#include "encoded_character_view.hpp"
#include "iterator.hpp"
#include "convert.hpp"

namespace u {

namespace internal {

	template<class C, class BaseIt>
	struct encoded_string_iterator_concept {

		static constexpr bool base_iterator_is_random_access
			= std::derived_from<u::iter_concept_t<BaseIt>, std::random_access_iterator_tag>;

		static constexpr bool codepoint_is_fixed_size
			= u::fixed_size_range_to_one_converter<typename C::decoder_type>;

		static constexpr bool is_random_access
			= base_iterator_is_random_access && codepoint_is_fixed_size;

		using type = std::conditional_t<
			is_random_access,
			std::random_access_iterator_tag,
			u::iter_concept_t<BaseIt>
		>;
	};

	template<class C, class BaseIt>
	requires(
		std::random_access_iterator<BaseIt>
		&&
		u::fixed_size_range_to_one_converter<C>
	)
	struct encoded_string_iterator_concept<C, BaseIt> {
		using type = std::random_access_iterator_tag;
	};
}

template<u::codec C, class It>
struct encoded_string_iterator
: u::iterator <
	typename internal::encoded_string_iterator_concept<C, It>::type,
	encoded_string_iterator<C, It>,
	u::value_type<u::encoded_character_view<C, It>>,
	u::difference_type<std::iter_difference_t<It>>,
	u::reference_type<u::encoded_character_view<C, It>>
> {
private:
	It m_it;

	using concept_chooser = typename internal::encoded_string_iterator_concept<C, It>;
	using base_type = u::iterator <
		typename concept_chooser::type,
		encoded_string_iterator<C, It>,
		u::value_type<u::encoded_character_view<C, It>>,
		u::difference_type<std::iter_difference_t<It>>,
		u::reference_type<u::encoded_character_view<C, It>>
	>;

	using typename base_type::difference_type;

public:

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
		auto copy = *this;
		++copy;
		return { m_it, copy.m_it };
	}

	auto& operator ++ ()
	requires ( not concept_chooser::codepoint_is_fixed_size ) {
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

	auto& operator += (difference_type n)
	requires concept_chooser::is_random_access {
		m_it += n * C::decoder_type::size;
		return *this;
	}

	difference_type operator - (const encoded_string_iterator& that) const
	requires concept_chooser::is_random_access {
		return m_it - that.m_it;
	}

	encoded_string_iterator operator - (difference_type that) const
	requires concept_chooser::is_random_access {
		return m_it - that;
	}
};

}