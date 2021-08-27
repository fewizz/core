#include "null_terminated_string_view.hpp"
#include "is.hpp"

#include <ranges>

static_assert(std::ranges::range<null_terminated_string_view<size_is::defined>>);
static_assert(std::ranges::range<null_terminated_string_view<size_is::undefined>>);

constexpr const char* str = "";
using from_ccp = decltype(null_terminated_string_view{str});
static_assert(is::type<from_ccp>::same_as<null_terminated_string_view<size_is::undefined>>);

using from_array = decltype(null_terminated_string_view{""});
static_assert(is::type<from_array>::same_as<null_terminated_string_view<size_is::defined>>);

int main() {
	static_assert(null_terminated_string_view{"asdasd"}.size() == 6);
}