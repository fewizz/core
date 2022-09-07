#include <variant.hpp>
#include <exchange.hpp>

consteval bool f() {
	{
		__variant::storage<int, float> storage;
		float f = 0;
		storage.init<__variant::treat_type_as::value_type>(f);
	}
	{
		variant<int, float> f{ 0.0F };
		if(!f.is<float>()) throw;
	}
	{
		variant<int, float> i{ 0 };
		if(!i.is<int>()) throw;
		i = 0.0F;
		if(!i.is<float>()) throw;
	}
	return true;
}

// static_assert(f());

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

struct c {
	bool moved_out;

	c() {
		moved_out = false;
	}

	c(c&& other) {
		other.moved_out = true;
	}
};

int main() {
	{
		variant<int, nuint> e{ nuint{ 666666666666 } };
		auto copy = e;
		if(!copy.is<nuint>()) throw;
		if(copy.get<nuint>() != nuint{ 666666666666 }) throw;

		auto moved = move(copy);
		if(!moved.is<nuint>()) throw;
		if(moved.get<nuint>() != nuint{ 666666666666 }) throw;
	}
	{
		variant<int, float> f{ 42.0F };
		if(!f.is<float>()) throw;
		if(f.get<float>() != 42.0F) throw;

		f = 42;

		if(!f.is<int>()) throw;
		if(f.get<int>() != 42) throw;

		variant<int, float> f0 = f;
	}

	{
		{
			variant<a, b> es{ a{ 4 } };
			if(!es.is<a>()) throw;
			if(es.get<a>().i != 4) throw;

			es = b{ 4.0F };

			if(!es.is<b>()) throw;
			if(es.get<b>().f != 4.0) throw;
		}
		if(a_destructions != 2) throw;
		if(b_destructions != 2) throw;
	}

	{
		int i = 0;
		variant<int&> int_ref{ i };
		int_ref.template get<int&>() = 1;
		if(int_ref.template get<int&>() != 1) throw;
	}

	{
		int i = 1;
		float f = 2.0;
		variant<int&, float&> x = i;
		if(&x.get<int&>() != &i) throw;

		x = f;
		if(&x.get<float&>() != &f) throw;
	}

	{
		static bool destructor_called = false;
		struct a {
			~a() {
				destructor_called = true;
			}
		};

		variant<int, float, a, bool> x = a{};
		x = 0.0F;
		if(!x.is<float>()) throw;
		if(!destructor_called) throw;
	}

	{
		c c0{};
		variant<c> x = move(c0);
		if(!c0.moved_out) throw;
	}
}