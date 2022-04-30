#include <core/concat.hpp>
#include <core/array.hpp>

consteval bool f_i() {
	concat_view v {
		array{ 0, 1, 2, 3 }, array{ -1, -10, -10000 }
	};

	auto it = v.begin();

	if(*it != 0) throw;
	if(*(++it) != 1) throw;
	if(*(++it) != 2) throw;
	if(*(++it) != 3) throw;
	if(*(++it) != -1) throw;
	if(*(++it) != -10) throw;
	if(*(++it) != -10000) throw;
	if(++it != v.end()) throw;

	return true;
}

static_assert(f_i());