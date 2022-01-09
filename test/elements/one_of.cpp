#include "elements/one_of.hpp"
#include "assert.h"

consteval void f() {
	{
		elements::one_of<int, float> f{ 0.0F };
		assert(f.is<float>());
	}
	{
		elements::one_of<int, float> i{ 0 };
		assert(i.is<int>());
		i = 0.0F;
		assert(i.is<float>());
	}
}

nuint a_destructions;

struct a {
	int i;

	~a() { ++a_destructions; }
};

nuint b_destructions;

struct b {
	float i;

	~b() { ++b_destructions; }
};

int main() {
	f();

	{
		elements::one_of<int, float> f{ 0.0F };
		assert(f.is<float>());
	}

	{
		elements::one_of<a, b> es{ 0 };
		assert(es.is<a>());
	}

	assert(a_destructions == 1);
	assert(b_destructions == 0);

	{ 
		elements::one_of<a, b> es{ 0.0F };
		assert(es.is<b>());
	}

	assert(a_destructions == 1);
	assert(b_destructions == 1);

	int i = 0;
	elements::one_of<int&> int_ref{ i };
	int_ref.template get<int&>() = 1;
	assert(int_ref.template get<int&>() == 1);
}