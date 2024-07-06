#include <write.hpp>
#include <array.hpp>

int main() {
	array<char, 4> a{};
	auto iter = a.iterator();

	write<uint8>(iter, 1);
	write<endianness::big>(iter, uint16{ 2 });
	write<uint8>(iter, 42);

	if (iter != a.sentinel()) {
		throw;
	}

	if (a[0] != 1 || a[1] != 0 || a[2] != 2 || a[3] != 42) {
		throw;
	}
}