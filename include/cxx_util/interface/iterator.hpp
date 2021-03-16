#pragma once

#include <compare>
#include <cstddef>
#include <iterator>
#include <memory>

namespace u {

template<class T>
struct difference_type {
	using type = T;
};

template<class T>
struct value_type {
	using type = T;
};

template<typename D>
struct post_incr_from_pre_incr {
	D operator ++ (int) {
		D& d = static_cast<D&>(*this);
		D copy { d };
		++d;
		return copy;
	}
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct input_or_output_iterator : post_incr_from_pre_incr<It> {
	static_assert(std::is_same_v<VT, u::value_type<typename VT::type>>);
	static_assert(std::is_same_v<DT, u::difference_type<typename DT::type>>);

	using value_type = typename VT::type;
	using difference_type = typename DT::type;
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct input_iterator : input_or_output_iterator<It, VT, DT> {
	using iterator_category = std::input_iterator_tag;
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct output_iterator : input_or_output_iterator<It, VT, DT> {
	using iterator_category = std::output_iterator_tag;
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct forward_iterator : input_or_output_iterator<It, VT, DT> {
	using iterator_category = std::forward_iterator_tag;
};

template<typename D>
struct post_decr_from_pre_decr {
	D operator -- (int) {
		D& d = static_cast<D&>(*this);
		D copy = d;
		++d;
		return copy;
	}
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct bidirectional_iterator
:
	post_incr_from_pre_incr<It>,
	post_decr_from_pre_decr<It>
{
	static_assert(std::is_same_v<VT, u::value_type<typename VT::type>>);
	static_assert(std::is_same_v<DT, u::difference_type<typename DT::type>>);

	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename VT::type;
	using difference_type = typename DT::type;
};

template<typename D>
struct pre_incr_from_add_assign {
	D& operator ++ () {
		return (static_cast<D&>(*this)) += 1;
	}
};

template<typename D>
struct pre_decr_from_sub_assign {
	D& operator -- () {
		return (static_cast<D&>(*this)) -= 1;
	}
};

template<typename D>
struct incr_and_decr_from_add_and_sub_assign
: 
	pre_incr_from_add_assign<D>,
	pre_decr_from_sub_assign<D>,
	post_incr_from_pre_incr<D>,
	post_decr_from_pre_decr<D>
{
	using pre_incr_from_add_assign<D>::operator ++;
	using post_incr_from_pre_incr<D>::operator ++;
	using pre_decr_from_sub_assign<D>::operator --;
	using post_decr_from_pre_decr<D>::operator --;
};

template<typename D>
struct sub_assign_from_add_assign {
	D& operator -= (const auto& n)  {
		return (static_cast<D&>(*this)) += (-n);
	}
};

template<typename D>
struct add_from_add_assign {
	D operator + (const auto& n) const {
		return D{(static_cast<const D&>(*this))} += n;
	}
};

template<typename D>
struct sub_from_sub_assign {
	D operator - (const auto& n) const {
		return D{(static_cast<const D&>(*this))} -= n;
	}
};

template<typename D>
struct subscipt_from_add_and_dereference {
	auto& operator [] (const auto& n) const {
		return *(static_cast<const D&>(*this) + n);
	}
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct random_access_iterator
:
	incr_and_decr_from_add_and_sub_assign<It>,
	sub_assign_from_add_assign<It>,
	add_from_add_assign<It>,
	sub_from_sub_assign<It>,
	subscipt_from_add_and_dereference<It>
{
	static_assert(std::is_same_v<VT, u::value_type<typename VT::type>>);
	static_assert(std::is_same_v<DT, u::difference_type<typename DT::type>>);

	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename VT::type;
	using difference_type = typename DT::type;
};

template<typename D, class value_type>
struct member_pointer_from_dereference {
	value_type* operator -> () {
		return std::to_address(static_cast<D&>(*this));
	}

	const value_type* operator -> () const {
		return std::to_address(static_cast<const D&>(*this));
	}
};

template<typename It, class VT, class DT = difference_type<std::ptrdiff_t>>
struct contiguous_iterator
:
	incr_and_decr_from_add_and_sub_assign<It>,
	sub_assign_from_add_assign<It>,
	add_from_add_assign<It>,
	sub_from_sub_assign<It>,
	subscipt_from_add_and_dereference<It>,
	member_pointer_from_dereference<It, typename VT::type>
{
	static_assert(std::is_same_v<VT, u::value_type<typename VT::type>>);
	static_assert(std::is_same_v<DT, u::difference_type<typename DT::type>>);

	using iterator_category = std::contiguous_iterator_tag;
	using element_type = typename VT::type;
	using difference_type = typename DT::type;
};

}