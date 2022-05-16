#include "core/meta/elements/one_of.hpp"

consteval void f() {
	{
		elements::one_of<int, float> f{ 0.0F };
		if(!f.is<float>()) throw;
	}
	{
		elements::one_of<int, float> i{ 0 };
		if(!i.is<int>()) throw;
		i = 0.0F;
		if(!i.is<float>()) throw;
	}
}

nuint a_destructions;

struct a {
	int i;
	a(int i) : i{i} {}
	~a() { ++a_destructions; }
};

nuint b_destructions;

struct b {
	float f;
	b(float f) : f{f} {}
	~b() { ++b_destructions; }
};

int main() {
	{
		elements::one_of<int, nuint> e{ nuint{ 666666666666 } };
		auto copy = e;
		if(!copy.is<nuint>()) throw;
		if(copy.get<nuint>() != nuint{ 666666666666 }) throw;

		auto moved = move(copy);
		if(!moved.is<nuint>()) throw;
		if(moved.get<nuint>() != nuint{ 666666666666 }) throw;
	}
	{
		elements::one_of<int, float> f{ 42.0F };
		if(!f.is<float>()) throw;
		if(f.get<float>() != 42.0F) throw;

		f = 42;

		if(!f.is<int>()) throw;
		if(f.get<int>() != 42) throw;

		elements::one_of<int, float> f0 = f;
	}

	{
		elements::one_of<a, b> es{ 4 };
		if(!es.is<a>()) throw;
		if(es.get<a>().i != 4) throw;

		es = b{ 4.0F };

		if(!es.is<b>()) throw;
		if(es.get<b>().f != 4.0) throw;
	}

	if(a_destructions != 1) throw;
	if(b_destructions != 2) throw;

	{
		int i = 0;
		elements::one_of<int&> int_ref{ i };
		int_ref.template get<int&>() = 1;
		if(int_ref.template get<int&>() != 1) throw;
	}
}