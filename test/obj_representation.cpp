#include "../include/cxx_util/obj_representation.hpp"
#include <cassert>
#include <type_traits>

int main() {
	auto check_front_and_back = [&](auto& c, int low) {
		bool big = std::endian::native == std::endian::big;
		assert(c.front() == std::byte( big ? 0 : low ) );
		assert(c.back() == std::byte( big ? low : 0 ) );
	};

	int num = 0x42;

	u::obj_representation rep{ num };
	assert(rep.size() == sizeof(int));

	check_front_and_back(rep, 0x42);
	num = 0xFF;
	check_front_and_back(rep, 0xFF);

	auto rep_copy = u::make_obj_representation_copy(num);
	check_front_and_back(rep_copy, 0xFF);
	num = 0x01;
	check_front_and_back(rep_copy, 0xFF);
}