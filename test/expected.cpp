#include <expected.hpp>

int main() {
	expected<int, float> f{1.5F};
	if(!f.is_unexpected()) throw 1;
	if(f.get_unexpected() != 1.5F) throw 2;

	{
		expected<int, int> expected_with_same_types{ 0 };
		if(!expected_with_same_types.is_expected()) { throw 3; }
	}
	{
		expected<int, int> expected_with_same_types{ unexpected{ 0 }};
		if(!expected_with_same_types.is_unexpected()) { throw 3; }
	}
	
}