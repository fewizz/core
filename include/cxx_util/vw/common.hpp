#pragma once

#include "string_def.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <bits/c++config.h>
#include <compare>
#include <type_traits>

namespace vw {

template<enc::encoding E, class T>
struct basic_string_view;

namespace internal {

template<class D, class E, class T>
struct string_common {
	using value_type = character_view<E, T>;
	using iterator = character_iterator<E, T>;
	using size_type = std::size_t;
private:
	constexpr T* data() const {
		return ((D*)this)->data();
	}

	constexpr size_type raw_size() const {
		return ((D*)this)->raw_size();
	}

public:

	static constexpr size_type npos = size_type(-1);

	constexpr iterator begin() const {
		return { data(), data(), data() + raw_size() };
	}

	constexpr iterator end() const {
		return { data(), data() + raw_size() };
	}

	constexpr size_type size() const {
		return std::distance(begin(), end());
	}

	constexpr size_type length() const {
		return size();
	}

	constexpr bool empty() const { return size() == 0; }

	constexpr value_type operator [] (size_type pos) const {
		auto it = begin();
		std::advance(it, pos);
		return *it;
	}

	constexpr value_type at(size_type pos) const {
		if(pos >= size()) throw std::runtime_error{ "vw::string_view::at" };
		auto it = begin();
		std::advance(it, pos);
		return *it;
	}

	constexpr value_type front() const {
		return *begin();
	}

	constexpr value_type back() const {
		auto it = begin();
		std::advance(it, size() - 1);
		return *it;
	}

	constexpr D substr(size_type pos = 0, size_type n = npos) const {
		auto b = begin();
		std::advance(b, pos);

		if(n == D::npos) n = size() - pos;

		auto e = b;
		std::advance(e, n);

		return { b, e };
	}

	constexpr size_type find(auto s, size_type pos = 0) const {
		auto it = begin();
		std::advance(it, pos);

		auto remains = size() - pos;
		auto that_size = s.size();

		while(remains >= that_size) {
			auto res = std::__memcmp(it.cur, s.data(), s.raw_size());
			if(res == 0) return pos;

			++pos;
			++it;
			--remains;
		}

		return npos;
	}

	constexpr size_type find(value_type c, size_type pos = 0) const {
		return find(basic_string_view<E, T> { c.data(), c.size() }, pos);
	}

	constexpr std::strong_ordering
	operator <=> (const string_common& that) const {
		return
		util::bytes { data(), data() + raw_size() }
		<=>
		util::bytes { that.data(), that.data() + that.raw_size() };
	}

	constexpr std::strong_ordering
	operator <=> (const std::ranges::range auto& that) const {
		return
		util::bytes { data(), data() + raw_size() }
		<=>
		util::bytes { that };
	}

	constexpr std::strong_ordering operator <=> (const T* that) const {
		return
		util::bytes { data(), data() + raw_size() }
		<=>
		util::bytes { std::basic_string_view<T> { that } };
	}

	constexpr bool
	operator == (const T* that) const {
		return *this <=> that == 0;
	}

	constexpr bool operator == (const string_common& that) const = default;
};

}

}