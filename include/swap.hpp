#pragma once

#include "./move.hpp"

template<typename A, typename B>
void swap(A& a, B& b) {
	A temp_a = move(a);
	a = move(b);
	b = move(temp_a);
}