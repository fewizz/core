#include <optional.hpp>

int main() {
	{
		optional<int> o;
		if(o.has_value()) return 1;
		o.if_has_value([](auto) { throw; });
		o = 1;
		if(o.has_no_value()) return 2;
		if(o.get() != 1) return 3;
	}
	{
		optional<int, float, bool> o;
		if(o.has_value()) return 4;
		o = 4.0F;
		if(o.has_no_value()) return 5;
		if(!o.is_same_as<float>()) return 6;
		if(o.get_same_as<float>() != 4.0F) return 7;
	}
	{
		int a = 54;
		optional<int&> o = a;
		a = 42;
		if(o.has_no_value()) return 8;
		if(o.get() != 42) return 9;
	}
	{
		int i = 1;
		int& ref = optional<int&>{ i }.get();
		if(ref != 1) return 10;
	}
	{
		if(optional<int>{ 0 }.get() != 0) return 11;
	}
}