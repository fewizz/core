#include "object/bytes.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iostream>

static_assert(std::ranges::range<u::obj::bytes<int>>);
static_assert(std::ranges::sized_range<u::obj::bytes<int>>);
static_assert(std::ranges::random_access_range<u::obj::bytes<int>>);

static_assert(std::ranges::range<u::obj::bytes_view<int>>);
static_assert(std::ranges::sized_range<u::obj::bytes_view<int>>);
static_assert(std::ranges::random_access_range<u::obj::bytes_view<int>>);
static_assert(std::ranges::viewable_range<u::obj::bytes_view<int>>);

int main() { // TODO check in compile-time
	bool big = std::endian::native == std::endian::big;

	auto check_front_and_back = [&](auto& c, int low) {
		assert(c.front() == std::byte( big ? 0 : low ) );
		assert(c.back() == std::byte( big ? low : 0 ) );
	};

	int num = 0x42;

	// create by reference
	u::obj::bytes_view rep{ num };
	assert(rep.size() == sizeof(int));
	// object copy creation from representation
	assert( rep.create() == 0x42 );

	check_front_and_back(rep, 0x42);
	num = 0xFF; // changes object representation
	check_front_and_back(rep, 0xFF);

	auto rep_copy = u::obj::bytes{ num };
	check_front_and_back(rep_copy, 0xFF);
	num = 0x01; // should not change copied object representation
	check_front_and_back(rep_copy, 0xFF);

	assert( rep_copy.create() == 0xFF );

	// convert to array
	auto int_bytes = u::obj::bytes<int>{ 0x44 };
	int_bytes[big ? sizeof(int) - 1 : 0] = std::byte{ 0x11 };

	// create from iterator
	assert(
		int_bytes.create()
		== 0x11
	);

	const int ci = 0;
	u::obj::bytes_view ci_bytes_ref{ ci };
	static_assert(std::is_same_v<decltype(ci_bytes_ref)::value_type, const std::byte>);

	u::obj::bytes_view ci_bytes_copy{ ci };
	static_assert(std::is_same_v<decltype(ci_bytes_copy)::value_type, const std::byte>);

	return 0;
}