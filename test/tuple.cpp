#include "tuple.hpp"
#include "is.hpp"
#include <utility>
#include <cassert>
#include <iostream>

int main() {
	int i = 0;
	float f = 10.0F;
	
	tuple t{ 0, i, 1, 0.0F, f};

	// size
	static_assert(t.size() == 5);

	// contains
	static_assert(t.contains<int>());
	static_assert(t.contains<int&>());
	static_assert(t.contains<float>());
	static_assert(t.contains<float&>());
	static_assert(! t.contains<char>());

	// count
	static_assert(t.count<int>() == 2);
	static_assert(t.count<int&>() == 1);

	// get element at
	auto& el = t.get_element_at<0>();
	assert(el == 0);
	el = 1;
	assert(t.get_element_at<0>() == 1);

	// get elements at
	auto floats = t.get_elements_at<3, 4>();
	assert(t.get_element_at<3>() == 0.0F);
	assert(t.get_element_at<4>() == 10.0F);
	
	++floats.get_element_at<0>();
	++floats.get_element_at<1>();
	
	assert(t.get_element_at<3>() == 1.0F);
	assert(t.get_element_at<4>() == 11.0F);

	// get
	int calls = 0;

	t.get([&](int&) {
		++calls;
	});

	assert(calls == 3);

	// move
	calls = 0;

	t.move(
		[&]<typename T>
		requires(is::type<T>::template same_as<int>)
		(T&&) {
			++calls;
		}
	);

	assert(calls == 3);
}