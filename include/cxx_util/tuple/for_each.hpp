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

namespace internal {

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

namespace internal {
	template<typename... Args, typename F, std::size_t... Indices>
	void for_each(std::tuple<Args...> t, F&& f, std::index_sequence<Indices...>) {
		u::for_each(std::forward<Args>(get<Indices>(t)) ... , std::forward<F>(f));
	}
} // internal

template<typename... Args, typename F>
void for_each(std::tuple<Args...> t, F&& f) {
	internal::for_each(std::forward<decltype(t)>(t), std::forward<F>(f), std::make_index_sequence<sizeof...(Args)>());
}

}