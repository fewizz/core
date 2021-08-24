#pragma once

#include <tuple>
#include <optional>
#include <utility>
#include "tuple/for_each.hpp"
#include "tuple/get.hpp"
#include "parameter_pack/parameter_pack.hpp"
#include "named.hpp"

namespace u {

template<typename T>
struct required {
	using value_type = T;

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> == 1);
	}
};

template<typename T>
struct several {
	using value_type = T;

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> >= 1);
	}
};

template<typename T>
struct optional {
	using value_type = T;

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> <= 1);
	}
};

template<typename T>
struct any {
	using value_type = T;
	struct param_requirement_mark{};

	template<typename... Ts>
	constexpr static void check() {}
};

template<typename... Ts>
struct params : std::tuple<Ts...> {

	using std::tuple<Ts...>::tuple;

	std::tuple<Ts...>& tuple() {
		return (std::tuple<Ts...>&) *this;
	}

	const std::tuple<Ts...>& tuple() const {
		return (std::tuple<Ts...>&) *this;
	}

	params(Ts&&... ts) : std::tuple<Ts...>{ std::forward<Ts>(ts)... } {}
	params(std::tuple<Ts...> t) : std::tuple<Ts...>{ std::move(t) } {}

	template<typename Req, typename F>
	auto handle(F&& f) const {
		Req::template check<Ts...>();

		using T = typename Req::value_type;
		using PP = u::parameter_pack<Ts...>;
		using indices_of_same = typename PP::template indices_of_same_as<T>;
		using indices_of_not_same = typename PP::template indices_of_not_same_as<T>;

		u::for_each(
			tuple(),
			indices_of_same{},
			std::forward<F>(f)
		);

		return u::params {
			u::get(std::move(tuple()), indices_of_not_same{})
		};
	}

	/*template<std::size_t... Indices>
	auto move(std::index_sequence<Indices...>) {
		return 
	}*/

	auto& check_for_emptiness() const {
		static_assert(sizeof...(Ts) == 0);
		return *this;
	}

	template<typename T>
	std::size_t count() const {
		return u::count<T, Ts...>;
	}

	template<typename T>
	bool has() const {
		return (this->template count<T>) > 0;
	}
};

template<typename... Ts>
params(Ts&&...) -> params<Ts...>;

template<typename... Ts>
params(std::tuple<Ts...>) -> params<Ts...>;

} // u