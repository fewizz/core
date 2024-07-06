#include <expected.hpp>
#include <expression_of_type.hpp>

static_assert(same_as<
	decltype(expression_of_type<expected<int, float>>().get_unexpected()),
	float&&
>);

static_assert(same_as<
	decltype(expression_of_type<expected<int, float>>().get_expected()),
	int&&
>);

int main() {
	expected<int, float> f{1.5F};
	if (!f.is_unexpected()) throw 1;
	if (f.get_unexpected() != 1.5F) throw 2;

	{
		expected<int, int> expected_with_same_types{ 0 };
		if (!expected_with_same_types.is_expected()) { throw 3; }
	}
	{
		expected<int, int> expected_with_same_types{ unexpected{ 0 }};
		if (!expected_with_same_types.is_unexpected()) { throw 3; }
	}
	
}