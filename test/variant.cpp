#include <variant.hpp>
#include <exchange.hpp>

consteval bool f() {
	{
		variant<int, int&> v{1};
		v.view([](int&) {});
		static_assert(same_as<
			decltype(v.get_at<0>()),
			int&
		>);
		static_assert(same_as<
			decltype(v.get_at<1>()),
			int&
		>);
	}
	{
		variant<int, float> f{ 0.0F };
		if(!f.is_same_as<float>()) throw;
	}
	{
		variant<int, float> i{ 0 };
		if(!i.is_same_as<int>()) throw;
		i = 0.0F;
		if(!i.is_same_as<float>()) throw;
	}
	return true;
}

// static_assert(f());

int main() {

	{
		variant<int, nuint> e{ nuint{ 666666666666 } };
		auto copy = e;
		if(!copy.is_same_as<nuint>()) throw;
		if(copy.get_same_as<nuint>() != nuint{ 666666666666 }) throw;

		auto moved = move(copy);
		if(!moved.is_same_as<nuint>()) throw;
		if(moved.get_same_as<nuint>() != nuint{ 666666666666 }) throw;
	}
	{
		variant<int, float> f{ 42.0F };
		if(!f.is_same_as<float>()) throw;
		if(f.get_same_as<float>() != 42.0F) throw;

		f = 42;

		if(!f.is_same_as<int>()) throw;
		if(f.get_same_as<int>() != 42) throw;

		variant<int, float> f0 = f;
	}

	{
		struct a {
			int i;
			nuint& a_destructions;
			a(int i, nuint& a_destructions) :
				i{i}, a_destructions{a_destructions}
			{}
			~a() { ++a_destructions; }
		};

		struct b {
			float f;
			nuint& b_destructions;
			b(float f, nuint& b_destructions) :
				f{f}, b_destructions{ b_destructions }
			{}
			~b() { ++b_destructions; }
		};

		nuint a_destructions = 0;
		nuint b_destructions = 0;

		{
			variant<a, b> es{ a{ 4, a_destructions } };
			if(!es.is_same_as<a>()) throw;
			if(es.get_same_as<a>().i != 4) throw;

			es = b{ 4.0F, b_destructions };

			if(!es.is_same_as<b>()) throw;
			if(es.get_same_as<b>().f != 4.0) throw;
		}
		if(a_destructions != 2) throw;
		if(b_destructions != 2) throw;
	}

	{
		int i = 0;
		variant<int&> int_ref{ i };
		int_ref.template get_same_as<int&>() = 1;
		if(int_ref.template get_same_as<int&>() != 1) throw;
	}

	{
		int i = 1;
		float f = 2.0;
		variant<int&, float&> x = i;
		if(&x.get_same_as<int&>() != &i) throw;

		x = f;
		if(&x.get_same_as<float&>() != &f) throw;
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
		if(!x.is_same_as<float>()) throw;
		if(!destructor_called) throw;
	}

	{
		struct c {
			bool moved_out;

			c() {
				moved_out = false;
			}

			c(c&& other) {
				other.moved_out = true;
			}
		};
		c c0{};
		variant<c> x = move(c0);
		if(!c0.moved_out) throw;
	}

	{
		variant<int, float> what{ variant_index<0>, 0.0F };
	}

	{
		static bool destroyed = false;

		struct check_destroy {
			~check_destroy() {
				destroyed = true;
			}
		};

		{
			variant<int, check_destroy>{ check_destroy{} };
		}

		if(!destroyed) throw;
	}
}