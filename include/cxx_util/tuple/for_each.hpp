#pragma once

#include <utility>
#include <tuple>
#include "../parameter_pack/parameter_pack.hpp"

namespace u {

template<typename... Ts>
struct do_one_of : Ts... {
	using Ts::operator()...;
};

template<typename... Ts>
do_one_of(Ts...) -> do_one_of<Ts...>;

constexpr inline auto do_nothing = [](const auto&){};

namespace internal {

	template<typename F>
	decltype(auto) for_each(F&& f) {
		return std::forward<F>(f);
	}

	template<typename Arg, typename F>
	decltype(auto) for_each(Arg&& a, F&& f) {
		f(std::forward<Arg>(a));
		return std::forward<F>(f);
	}

	template<typename... Ts>
	requires(sizeof...(Ts) >= 2)
	decltype(auto) for_each(auto&& a, Ts&&...values) {
		decltype(auto) f = for_each(std::forward<Ts>(values)...);
		f(a);
		return std::forward<decltype(f)>(f);
	}
}

template<typename... Args>
void for_each(Args&&... args) {
	internal::for_each(std::forward<Args>(args)...);
}

template<typename... Args, std::size_t... Indices, typename F>
void for_each(std::tuple<Args...> t, std::index_sequence<Indices...>, F&& f) {
	u::for_each(
		get<Indices>(t) ... ,
		std::forward<F>(f)
	);
}

template<typename... Args, typename F>
void for_each(std::tuple<Args...> t, F&& f) {
	u::for_each(
		std::forward<decltype(t)>(t),
		std::make_index_sequence<sizeof...(Args)>(),
		std::forward<F>(f)
	);
}

}