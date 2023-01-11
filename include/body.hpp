#pragma once

#include <handle.hpp>
#include <exchange.hpp>

template<typename Type>
class body {

	handle<Type> soul_handle_{};

public:

	body() {}

	body(typename handle<Type>::underlying_type underlying) :
		soul_handle_{ underlying }
	{}

	body(body&& other) :
		soul_handle_{ move(other.soul_handle_) }
	{}
	body(const body& other) = delete;

	body& operator = (body&& other) {
		soul_handle_ = move(other.soul_handle_);
		return *this;
	}

	body& operator = (const body& other) = delete;

	~body() {
		if(soul_handle_.is_valid()) {
			destroy();
		}
	}

	const ::handle<Type>* operator -> () const { return &soul_handle_; }
	      ::handle<Type>* operator -> ()       { return &soul_handle_; }

	void destroy() {
		do_destroy();
		soul_handle_.invalidate();
	}

	inline void do_destroy();

};