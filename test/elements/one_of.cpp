#include "elements/one_of.hpp"
#include "../assert.hpp"
#include "type/is_constructible_from.hpp"

struct a {
	int i;
};

struct b {
	float i;
};

int main() {
	elements::one_of<int, float> f_{ 0.0F };

	assert(f_.is_current<float>());

	elements::one_of<a, b> a_{ 0 };
	assert(a_.is_current<a>());

	elements::one_of<a, b> b_{ 0.0F };
	assert(b_.is_current<b>());

	int i = 0;
	elements::one_of<int&> int_ref{ i };
	int_ref.template get<int&>() = 1;
	assert(int_ref.template get<int&>() == 1);
}