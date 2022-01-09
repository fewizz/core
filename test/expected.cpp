#include "expected.hpp"

int main() {
	expected<int, float> f{0.0F};
	f.handle_if_unexpected([](float){});

	int val = f;
	++val;
}