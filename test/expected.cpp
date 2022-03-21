#include "core/expected.hpp"

int main() {
	expected<int, float> f{0.0F};

	int val = f;
	++val;
}