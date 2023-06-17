#include <output_stream.hpp>
#include <integer.hpp>
#include <write.hpp>

struct simple_os {
	nuint bytes_written = 0;

	constexpr void write(uint1a) {
		++bytes_written;
	}

};

consteval bool f() {
	simple_os os{};
	write(0, os);
	if(os.bytes_written != 1) throw;

	write<endianness::big, int>(0, os);

	if(os.bytes_written != 1 + sizeof(int)) throw;

	return true;
}

static_assert(f());

int main() { return 0; }