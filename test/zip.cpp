#include <core/zip.hpp>
#include <core/array.hpp>
#include <core/c_string.hpp>

constexpr array a{ 1,   4,   9,    16     };
constexpr array b{ 'c', 'a', 's',  'j'    };
constexpr array c{ 0.1, 1.0, 10.0, 1000.0 };

consteval auto f() {
	return zip_view {
		a, b, c,
		c_string{ "hell" }
	};
}

static_assert(f()[0] == elements::of{ 1,  'c', 0.1,    'h' });
static_assert(f()[1] == elements::of{ 4,  'a', 1.0,    'e' });
static_assert(f()[2] == elements::of{ 9,  's', 10.0,   'l' });
static_assert(f()[3] == elements::of{ 16, 'j', 1000.0, 'l' });

static_assert(f().begin() + 4 == f().end());
static_assert(f().begin() + 3 != f().end());

int main() {}