#include <expected.hpp>

int main() {
	expected<int, float> f{0.0F};
	[[maybe_unused]] int val = f;
	++val;
}