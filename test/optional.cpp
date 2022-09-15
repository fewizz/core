#include <optional.hpp>

int main() {
	{
		optional<int> o;
		if(o.has_value()) return 1;
		o.if_has_value([](auto) { throw; });
		o = 1;
		if(o.has_no_value()) return 2;
		if(o.value() != 1) return 3;
	}
	{
		optional<int, float, bool> o;
		if(o.has_value()) return 4;
		o = 4.0F;
		if(o.has_no_value()) return 5;
		if(!o.is<float>()) return 6;
		if(o.get<float>() != 4.0F) return 7;
	}
}