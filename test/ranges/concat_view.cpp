#include <__ranges/concat_view.hpp>
#include <array.hpp>
#include <c_string.hpp>

consteval bool f_0() {
	{
		[[maybe_unused]] __ranges::concat_view v {
			c_string{ "" }, array{ -1, -10, -10000 }
		};
	}

	__ranges::concat_view v {
		array{ 0, 1, 2, 3 }, array{ -1, -10, -10000 }
	};

	static_assert(same_as<
		decltype(*v.iterator()),
		int&
	>);

	auto it = v.iterator();

	if(v.size() != 7) throw;

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

	__ranges::concat_view v2 {
		array{ 'a' }, c_string{ "ka" }, c_string{ "s" }, array{ 'i' }
	};
	if(v2.size() != 5) throw;
	if(*v2.iterator() != 'a') throw;
	if(*(v2.iterator() + 1) != 'k') throw;
	if(*(v2.iterator() + 2) != 'a') throw;
	if(*(v2.iterator() + 3) != 's') throw;
	if(*(v2.iterator() + 4) != 'i') throw;
	if((v2.iterator() + 5) != v2.sentinel()) throw;
	return true;
}

static_assert(f_0());

int main() {
	__ranges::concat_view v2 {
		array{ 'a' }, c_string{ "ka" }, c_string{ "s" }, array{ 'i' }
	};
	if(v2.size() != 5) throw;
}