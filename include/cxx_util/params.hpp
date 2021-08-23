#pragma once

#include <tuple>
#include <optional>
#include "tuple/for_each.hpp"
#include "tuple/get.hpp"
#include "parameter_pack/parameter_pack.hpp"
#include "named.hpp"

namespace u {

template<typename T>
struct required_single : u::named<T> {
	using value_type = T;
	struct param_requirement_mark{};

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> == 1);
	};
};

template<typename T>
struct required {
	using value_type = T;
	struct param_requirement_mark{};

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> >= 1);
	};
};

template<typename T>
struct optional {
	using value_type = T;
	struct param_requirement_mark{};

	template<typename... Ts>
	constexpr static void check() {
		static_assert(u::count<T, Ts...> <= 1);
	};
};

template<typename... Ts>
struct params : std::tuple<Ts...> {

	using std::tuple<Ts...>::tuple;
	params(const std::tuple<Ts...>& v) : std::tuple<Ts...>{ v } {}

	template<typename Req, typename F>
	auto handle(F&& f) const {
		Req::template check<Ts...>();

		using T = typename Req::value_type;
		using PP = u::parameter_pack<Ts...>;
		using indices_of_same = typename PP::template indices_of_same_as<T>;
		using indices_of_not_same = typename PP::template indices_of_not_same_as<T>;

		u::for_each(
			*((std::tuple<Ts...>*)this),
			indices_of_same{},
			std::forward<F>(f)
		);

		return u::params{ u::get(*((std::tuple<Ts...>*)this), indices_of_not_same{}) };
	}

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
params(Ts...) -> params<Ts...>;

template<typename... Ts>
params(std::tuple<Ts...>) -> params<Ts...>;

} // u