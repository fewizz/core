#pragma once

#include <utility>
#include "parameter_pack.hpp"

namespace u {

//struct do_one_of_mark {};

template<typename... Ts>
struct do_one_of : Ts... {//}, do_one_of_mark {
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
	decltype(auto) for_each(auto&& a, Ts...values) {
		decltype(auto) f = for_each(std::forward<Ts>(values)...);
		f(a);
		return std::forward<decltype(f)>(f);
	}
}

template<typename... Args>
void for_each(Args&&... args) {
	internal::for_each(std::forward<Args>(args)...);
}

}