#include "checking_iterator.hpp"
#include <cassert>
#include <iterator>
#include <stdexcept>

static_assert(std::input_iterator<u::checking_iterator<int*>>);
static_assert(std::output_iterator<u::checking_iterator<int*>, int>);
static_assert(std::bidirectional_iterator<u::checking_iterator<int*>>);
static_assert(std::random_access_iterator<u::checking_iterator<int*>>);
static_assert(std::contiguous_iterator<u::checking_iterator<int*>>);

int main() {
	int i = 0;

	u::checking_iterator<int*> it{&i, &i + 1};
	++it;
	assert(it.distance_from_begin() == 1);

	bool catched = false;

	try {
		++it;
	} catch(std::out_of_range& e) {
		catched = true;
	}

	assert( catched );
	assert(it.distance_from_begin() == 1);

	--it;
	assert(it.distance_from_begin() == 0);

	catched = false;
	try {
		--it;
	} catch(std::out_of_range& e) {
		catched = true;
	}

	assert( catched );
	assert( it.distance_from_begin() == 0);
}