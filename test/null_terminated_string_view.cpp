#include "null_terminated_string_view.hpp"

#include <ranges>

static_assert(std::ranges::range<u::null_terminated_string_view<u::size_is::defined>>);
static_assert(std::ranges::range<u::null_terminated_string_view<u::size_is::undefined>>);

constexpr const char* str = "";
using from_ccp = decltype(u::null_terminated_string_view{str});
static_assert(std::is_same_v<from_ccp, u::null_terminated_string_view<u::size_is::undefined>>);

using from_array = decltype(u::null_terminated_string_view{""});
static_assert(std::is_same_v<from_array, u::null_terminated_string_view<u::size_is::defined>>);

int main() {
	static_assert(u::null_terminated_string_view{"asdasd"}.size() == 6);
}