#include <tuple.hpp>
#include <move.hpp>
#include <expression_of_type.hpp>

struct a {
	constexpr a() = default;
	constexpr a(a&&) = default;
	constexpr a(const a&) = delete;
};

consteval tuple<int, float, bool> some_elements() {
	return { 1, 0.0F, false };
}

consteval int f() {
	tuple elems{ 0, 1.0F, a{} };

	{
		bool passed = false;
		elems.pass_satisfying_predicate<is_same_as<float>>([&](float) {
			passed = true;
		});
		if (!passed) throw;
	}

	[[maybe_unused]] auto elems1 = move(elems);

	[[maybe_unused]] auto [ i, f, b ] = some_elements();

	return 0;
}

static_assert(f() == 0);

static_assert(
	same_as<
		decltype(expression_of_type<tuple<int, int&>>().get_at<0>()),
		int&&
	>
);
static_assert(
	same_as<
		decltype(move(expression_of_type<tuple<int, int&>>().get_at<0>())),
		int&&
	>
);
static_assert(
	same_as<
		decltype(expression_of_type<tuple<int, int&>>().get_at<1>()),
		int&
	>
);
static_assert(
	same_as<
		decltype(move(expression_of_type<tuple<int, int&>>()).get_at<1>()),
		int&
	>
);

int main() {}