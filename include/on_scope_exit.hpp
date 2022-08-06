#pragma once

#include "forward.hpp"

template<typename Handler>
class on_scope_exit {
	Handler handler_;
public:

	constexpr on_scope_exit(Handler&& handler) :
		handler_{ forward<Handler>(handler) }
	{}

	constexpr ~on_scope_exit() {
		handler_();
	}

};

template<typename Handler>
on_scope_exit(Handler&&) -> on_scope_exit<Handler>;