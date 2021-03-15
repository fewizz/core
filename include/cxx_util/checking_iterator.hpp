#pragma once

#include <compare>
#include <iterator>
#include <memory>
#include <stdexcept>
#include "iterator.hpp"
#include "interface/iterator.hpp"

namespace u {

template<class It>
struct checking_iterator
:
	u::incr_and_decr_from_add_and_sub_assign<checking_iterator<It>>,
	u::sub_assign_from_add_assign<checking_iterator<It>>,
	u::add_from_add_assign<checking_iterator<It>>,
	u::sub_from_add_assign<checking_iterator<It>>
{
	using this_type = checking_iterator<It>;

	using u::incr_and_decr_from_add_and_sub_assign<this_type>::operator ++;
	using u::incr_and_decr_from_add_and_sub_assign<this_type>::operator --;

	using base_iterator_type = It;
	using value_type = std::iter_value_t<It>;
	using reference = std::iter_reference_t<It>;
	using pointer = typename std::iterator_traits<It>::pointer;
	using difference_type = std::iter_difference_t<It>;
	using iterator_category = typename std::__detail::__iter_concept<It>;

private:

	It m_begin;
	It m_current;
	It m_end;

public:

	checking_iterator() = default;

	checking_iterator(const checking_iterator&) = default;
	checking_iterator&
	operator = (const checking_iterator&) = default;

	checking_iterator(checking_iterator&&) = default;
	checking_iterator&
	operator = (checking_iterator&&) = default;

	checking_iterator(It begin, It end)
	: m_begin{ begin }, m_current{ begin }, m_end{ end } {}

	It& base_begin() {
		return m_begin;
	}

	It& base() {
		return m_current;
	}

	It& base_end() {
		return m_end;
	}

	auto base_to_end_subrange() const {
		return std::ranges::subrange(m_current, m_end);
	}

	auto begin_to_base_subrange() const {
		return std::ranges::subrange(m_begin, m_current);
	}

	reference operator * () const {
		return *m_current;
	}

	difference_type operator - (It that_base) const {
		return { m_current - that_base };
	}

	difference_type operator - (checking_iterator that) const {
		return { m_current - that.m_current };
	}

	using sub_from_add_assign<this_type>::operator -;

	auto& operator += (difference_type n) {
		auto res = u::advance(m_current, m_begin, m_end, n);
		if(res) {
			m_current = res.value();
		}
		else {
			throw std::out_of_range{ "passed the end" };
		}
		return *this;
	}

	std::strong_ordering
	operator <=> (const checking_iterator& that) const {
		return std::compare_three_way{}(m_current, that.m_current);
	}

	std::strong_ordering
	operator <=> (const It& that_base) const {
		return std::compare_three_way{}(m_current, that_base);
	}

	bool operator == (const auto& that) const {
		return (*this <=> that) == 0;
	}

	reference operator [] (difference_type n) const {
		if(m_current + n > m_end)
			throw std::out_of_range{ "passed the end" };
		return m_current[n];
	}

	pointer operator -> () const {
		return std::to_address(m_current);
	}
};

template<class It>
checking_iterator<It> operator + (
	typename checking_iterator<It>::difference_type n,
	checking_iterator<It> it
) {
	auto i = n + it.base();
	return { i, it.base_end() };
}

}