#include <core/meta/decayed_same_as.hpp>
#include <core/meta/elements/pass_not_satisfying_type_predicate.hpp>
#include <core/meta/types/are_exclusively_satisfying_predicates.hpp>

template<typename Type> struct w { Type value; };

template<nuint Order, typename... Args>
constexpr auto f(Args&&... args) {
	return f<Order + 1, Args...>(forward<Args>(args)...);
}

template<nuint Order, typename... Args>
requires (Order == 2) && types::are_exclusively_satisfying_predicates<
	types::are_contain_one_decayed<w<int>>,
	types::are_contain_one_decayed<w<float>>
>::for_types<Args...>
constexpr auto f(Args&&...) {
	return 42;
}

template<nuint Order, typename... Args>
requires (Order == 1) &&
	types::are_contain_one_decayed<float>::for_types<Args...>
constexpr auto f(Args&&... args) {
	return elements::pass_not_satisfying_type_predicate<
		type::is_same_as<float>
	>(
		w<float>{ 1.0 },
		forward<Args>(args)...
	)(
		[]<typename... Args0>(Args0... args) {
			return f<2, Args0...>(forward<Args0>(args)...);
		}
	);
}

template<nuint Order, typename... Args>
requires (Order == 0) &&
	types::are_contain_one_decayed<int>::for_types<Args...>
constexpr auto f(Args&&... args) {
	return elements::pass_not_satisfying_type_predicate<
		type::is_same_as<int>
	>(
		w<int>{ 1 },
		forward<Args>(args)...
	)(
		[]<typename... Args0>(Args0... args) {
			return f<1, Args0...>(forward<Args0>(args)...);
		}
	);
}

template<typename... Args>
constexpr auto f(Args&&... args) {
	return f<0, Args...>(forward<Args>(args)...);
}

static_assert(f(0, 0.0F) == 42);
static_assert(f(w<int>{0}, 0.0F) == 42);
static_assert(f(0, w<float>{0.0F}) == 42);
static_assert(f(w<int>{0}, w<float>{0.0F}) == 42);