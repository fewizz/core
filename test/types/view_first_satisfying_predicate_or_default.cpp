#include <types.hpp>
#include <type.hpp>

consteval bool f() {
	using ts = types<float, bool, int, double>;

	nuint checks = 0;

	auto val = ts::view_first_satisfying_predicate_or_default(
		[&]<typename Type>() {
			++checks;
			return same_as<Type, int>;
		},
		[]<typename Type>(){ return int(same_as<Type, int>); },
		[](){ return 0; }
	);

	if(val != 1) throw;
	if(checks != 3) throw;

	return true;
}

static_assert(f());

int main() {}