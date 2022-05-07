#pragma once

#include "move.hpp"

template<typename Handler>
class on_scope_exit {
	Handler handler_;
public:

	constexpr on_scope_exit(Handler handler) :
		handler_{ handler }
	{}

	constexpr ~on_scope_exit() {
		handler_();
	}

};