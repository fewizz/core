#include <ranges/concat_view.hpp>
#include <array.hpp>
#include <c_string.hpp>

consteval bool f_i() {
	{
		[[maybe_unused]] ranges::concat_view v {
			c_string{ "" }, array{ -1, -10, -10000 }
		};
	}

	ranges::concat_view v {
		array{ 0, 1, 2, 3 }, array{ -1, -10, -10000 }
	};

	auto it = v.iterator();

	if(v.iterator() != v.iterator()) throw;
	if(v.iterator() + 1 != v.iterator() + 1) throw;

	if(*it != 0) throw;
	if(*(++it) != 1) throw;
	if(it != v.iterator() + 1) throw;
	if(*(++it) != 2) throw;
	if(it != v.iterator() + 2) throw;
	if(*(++it) != 3) throw;
	if(it != v.iterator() + 3) throw;
	if(*(++it) != -1) throw;
	if(it != v.iterator() + 4) throw;
	if(*(++it) != -10) throw;
	if(it != v.iterator() + 5) throw;
	if(*(++it) != -10000) throw;
	if(it != v.iterator() + 6) throw;
	if(++it != v.sentinel()) throw;

	it = v.iterator();
	if(it != v.iterator()) throw;
	if(*(it += 2) != 2) throw;
	if(it != v.iterator() + 2) throw;
	if(*(it += 2) != -1) throw;
	if(it != v.iterator() + 4) throw;
	if(*(it += 2) != -10000) throw;
	if(it != v.iterator() + 6) throw;
	it += 1;
	if(it != v.iterator() + 7) throw;

	if(it - v.iterator() != 7) throw;

	if(it != v.sentinel()) throw;

	return true;
}

static_assert(f_i());

int main() {}