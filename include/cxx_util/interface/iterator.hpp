#pragma once

#include <compare>

namespace u {

template<class D>
struct post_incr_from_pre_incr {
	D operator ++ (int) {
		D& d = static_cast<D&>(*this);
		D copy { d };
		++d;
		return copy;
	}
};

template<class D>
struct post_decr_from_pre_decr {
	D operator -- (int) {
		D& d = static_cast<D&>(*this);
		D copy = d;
		++d;
		return copy;
	}
};

template<class D>
struct pre_incr_from_add_assign {
	D& operator ++ () {
		return (static_cast<D&>(*this)) += 1;
	}
};

template<class D>
struct pre_decr_from_sub_assign {
	D& operator -- () {
		return (static_cast<D&>(*this)) -= 1;
	}
};

template<class D>
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

template<class D>
struct sub_assign_from_add_assign {
	D& operator -= (const auto& n)  {
		return (static_cast<D&>(*this)) += (-n);
	}
};

template<class D>
struct add_from_add_assign {
	D operator + (const auto& n) const {
		return D{(static_cast<const D&>(*this))} += n;
	}
};

template<class D>
struct sub_from_add_assign {
	D operator - (const auto& n) const {
		return D{(static_cast<const D&>(*this))} += (-n);
	}
};

}