#pragma once

//#include "character.hpp"
#include <bits/stdint-uintn.h>
#include <compare>
#include <cstddef>
#include <iterator>
#include "character.hpp"
#include "string_def.hpp"

namespace vw {

template<enc::encoding Encoding, class T>
struct character_iterator {
	const T* begin;
	const T* cur;
	const T* end;

	using difference_type = std::ptrdiff_t;
	using value_type = character_view<Encoding, T>;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::forward_iterator_tag;

	character_iterator() {}
	character_iterator(character_iterator&& other) = default;
	character_iterator& operator = (character_iterator&& other) = default;
	character_iterator(const character_iterator& other) = default;
	character_iterator& operator = (const character_iterator& other) = default;

private:
	constexpr void check() const {
		if(begin > cur)
			throw std::out_of_range{ "begin passed current" };
		if(cur > end)
			throw std::out_of_range{ "current passed end" };
	}

	constexpr auto width() const {
		return /*auto w =*/ enc::size<Encoding>(cur, end);
		//if(!w) throw std::runtime_error{ "getting multibyte character width" };
		//return w.value();
	}
public:
	constexpr character_iterator(
		const T* begin,
		const T* cur,
		const T* end
	) :
	begin{ begin }, cur{ cur }, end{ end } {
		check();
	}

	constexpr character_iterator(
		const T* begin, const T* end
	) :
	begin{ begin }, cur{ end }, end{ end } {}

	constexpr value_type operator * () const {
		check();
		return { cur, width() };
	}

	constexpr auto& operator ++ () {
		check();
		cur += width();
		return *this;
	}

	constexpr auto operator ++ (int) {
		auto prev = *this;
		++(*this);
		return prev;
	}

	/*constexpr auto operator + (std::integral auto offset) const {
		auto offset0 = offset;
		auto it = cur;

		if(it < end) {
			while(offset-- > 0) {
				it += width();
				if(it >= end) break;
			}

			return character_iterator<Encoding, T>{ begin, it, end };
		}

		throw std::out_of_range{
			"requested offset: " + std::to_string(offset0)
		};
	}*/

	constexpr std::strong_ordering
	operator <=>(const character_iterator& other) const {
		return cur <=> other.cur;
	}

	constexpr bool operator == (const character_iterator& other) const = default;
	//bool operator != (const character_iterator& other) const = default;*/
};

}