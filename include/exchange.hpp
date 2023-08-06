#pragma once

#include "./move.hpp"

template<typename A>
A exchange(A& a, auto&& b) {
	A prev = move(a);
	a = move(b);
	return move(prev);
}