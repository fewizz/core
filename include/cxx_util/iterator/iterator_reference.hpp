#pragma once

#include "iterator.hpp"
#include <compare>
#include <iterator>
#include <variant>

namespace u {

template<class It>
class iterator_reference {
	std::variant<It, It*> m_it;

	using traits = std::iterator_traits<It>;
public:

	using iterator_category = typename traits::iterator_category;
	using value_type = std::iter_value_t<It>;
	using difference_type = std::iter_difference_t<It>;

	constexpr iterator_reference() = default;
	constexpr iterator_reference(It& it) : m_it{ &it } {}
	constexpr iterator_reference(It&& it) : m_it{ std::move(it) } {}
	constexpr iterator_reference(const iterator_reference&) = default;

	constexpr It& base() {
		return m_it.index() == 0 ? std::get<0>(m_it) : *(std::get<1>(m_it));
	}

	constexpr const It& base() const {
		return m_it.index() == 0 ? std::get<0>(m_it) : *(std::get<1>(m_it));
	}

	constexpr value_type operator * () const {
		return *base();
	}

	constexpr auto operator -> () const
	requires (std::is_pointer_v<It> || requires (const It i) { i.operator->(); }) {
		return *base();
	}

	constexpr value_type operator [] (difference_type n) const {
		return base()[n];
	}

	constexpr auto& operator ++ () {
		++base();
		return *this;
	}

	constexpr auto& operator -- () {
		--base();
		return *this;
	}

	constexpr iterator_reference operator ++ (int) {
		return { base()++ };
	}

	constexpr iterator_reference operator -- (int) {
		return { base()-- };
	}

	constexpr iterator_reference operator + (difference_type n) const {
		return { base() + n };
	}

	constexpr iterator_reference operator - (difference_type n) const {
		return { base() - n };
	}

	constexpr auto& operator += (difference_type n) {
		base() += n;
		return *this;
	}

	constexpr auto& operator -= (difference_type n) {
		base() -= n;
		return *this;
	}

	constexpr bool operator == (const iterator_reference& that) const {
		return (m_it.index() == -1 && that.m_it.index() == -1) || (base() == that.base());
	}

	constexpr std::strong_ordering operator <=> (const iterator_reference&) const = default;
};

template<class It>
constexpr iterator_reference<It> operator + (typename iterator_reference<It>::difference_type n, const iterator_reference<It>& it) {
	return { n + it.base() };
}

template<class It>
constexpr auto operator - (const iterator_reference<It>& a, const iterator_reference<It>& b) {
	return a.base() - b.base();
}

}