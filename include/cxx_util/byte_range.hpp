#pragma once

#include "byte_iterator.hpp"
#include "object.hpp"
#include <iterator>

namespace u {

template<class It>
struct byte_range {
	using iterator = u::byte_iterator<It>;

private:
	iterator m_begin;
	iterator m_end;

public:

	using size_type = std::size_t;

	constexpr byte_range(It begin, It end)
	:	m_begin { begin },
		m_end { end }
		{}

	template<std::ranges::range R>
	constexpr byte_range(R& range)
	:
		m_begin { std::ranges::begin(range) },
		m_end { std::ranges::end(range) }
		{}

	template<class T, std::size_t N>
	constexpr byte_range(T (&arr )[N])
	: byte_range(arr, arr + N) {}

	constexpr iterator begin() const { return m_begin; }
	constexpr iterator end() const { return m_end; }

	constexpr size_type size() const
	requires( std::contiguous_iterator<It> ) {
		return std::distance(m_begin, m_end);
	}

	template<class It0>
	constexpr std::strong_ordering
	operator <=> (const byte_range<It0>& that) const {
		if(auto s = size() <=> that.size(); s != 0) return s;

		auto other_it = that.begin();

		for(auto v : *this) {
			auto other_v = *other_it;

			if(auto s = v <=> other_v; s != 0) return s;
			++other_it;
		}

		return std::strong_ordering::equal;
	}

	template<class It0>
	constexpr bool
	operator == (const byte_range<It0>& that) const {
		return (*this <=> that) == 0;
	};

	template<class It0>
	void copy_to(It0 it) const {
		using val_type = std::iter_value_t<It0>;

		auto this_is = begin();

		while(this_is < end()) {
			val_type v{};

			for(std::byte& b : u::obj_representation_copy{v}) {
				b = *(this_is++);
			}

			*it++ = v;
		}
	}
};

// deduction guides
template<std::ranges::range R>
byte_range(R& range) -> byte_range<std::ranges::iterator_t<R>>;

template<class T, std::size_t N>
byte_range(T (&)[N]) -> byte_range<T*>;

}