#include <input_stream.hpp>
#include <integer.hpp>
#include <read.hpp>

struct simple_is {
	nuint reads = 0;

	constexpr uint1a read() {
		++reads;
		return 0;
	}

};

static_assert(basic_input_stream<simple_is, uint1a>);

consteval bool f() {
	simple_is is{};

	::read<uint1a>(is);

	if(is.reads != 1) throw;

	read<int, endianness::big>(is);

	if(is.reads != 1 + sizeof(int)) throw;

	char arr[4]{ 0, 0, 0, 42 };

	static_assert(basic_iterator<decltype((char*) arr)>);

	if(read<int, endianness::big>((char*) arr) != 42) throw;

	return true;
}

static_assert(f());

int main() {}