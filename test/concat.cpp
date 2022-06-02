#include <core/concat.hpp>
#include <core/array.hpp>
#include <core/c_string.hpp>

consteval bool f_i() {
	{
		[[maybe_unused]] concat_view v {
			c_string{ "" }, array{ -1, -10, -10000 }
		};
	}

	concat_view v {
		array{ 0, 1, 2, 3 }, array{ -1, -10, -10000 }
	};

	auto it = v.begin();

	if(v.begin() != v.begin()) throw;
	if(v.begin() + 1 != v.begin() + 1) throw;

	if(*it != 0) throw;
	if(*(++it) != 1) throw;
	if(it != v.begin() + 1) throw;
	if(*(++it) != 2) throw;
	if(it != v.begin() + 2) throw;
	if(*(++it) != 3) throw;
	if(it != v.begin() + 3) throw;
	if(*(++it) != -1) throw;
	if(it != v.begin() + 4) throw;
	if(*(++it) != -10) throw;
	if(it != v.begin() + 5) throw;
	if(*(++it) != -10000) throw;
	if(it != v.begin() + 6) throw;
	if(++it != v.end()) throw;

	it = v.begin();
	if(it != v.begin()) throw;
	if(*(it += 2) != 2) throw;
	if(it != v.begin() + 2) throw;
	if(*(it += 2) != -1) throw;
	if(it != v.begin() + 4) throw;
	if(*(it += 2) != -10000) throw;
	if(it != v.begin() + 6) throw;
	it += 1;
	if(it != v.begin() + 7) throw;

	if(it - v.begin() != 7) throw;

	if(it != v.end()) throw;

	return true;
}

static_assert(f_i());

int main() {}