#pragma once

#include <handle.hpp>

template<typename Type>
class body {

	handle<Type> soul_handle_;

public:

	body(typename handle<Type>::underlying_type underlying) :
		soul_handle_{ underlying }
	{}

	body(body&& other) : soul_handle_{ other.soul_handle_ } {
		other.soul_handle_.invalidate();
	}

	~body() {
		if(soul_handle_.is_valid()) {
			destroy();
		}
	}

	::handle<Type>* operator -> () {
		return &soul_handle_;
	}


	constexpr void destroy() {
		do_destroy();
		soul_handle_.invalidate();
	}

	constexpr void do_destroy();

};