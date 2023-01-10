#include <expected.hpp>

int main() {
	expected<int, float> f{1.5F};
	if(!f.is_unexpected()) throw 1;
	if(f.get_unexpected() != 1.5F) throw 2;
}