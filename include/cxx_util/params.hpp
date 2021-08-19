#pragma once

#include <tuple>
#include <optional>
#include "tuple/for_each.hpp"

namespace u {

template<typename... Ps>
struct params : std::tuple<std::optional<Ps>...> {
	using std::tuple<std::optional<Ps>...>::tuple;

	template<typename T, std::size_t... Indexes>
	constexpr auto get() {
		if constexpr(sizeof...(Indexes) != 0) {
			std::tuple result{ std::get<Indexes>(*this)... };
			(std::get<Indexes>(*this).reset() , ...);
			return result;
		}
		else {
			if constexpr(std::is_same_v<T, )
			return get<T, Indexes..., sizeof...(Indexes)>();
		}
	}

	constexpr std::size_t size() {
		std::size_t result = 0;

		u::for_each(
			*this,
			[&]<typename T>(std::optional<T>& o) { if(o.has_value()) ++result; }
		);

		return result;
	}
}; // params

template<typename T>
struct one {
	template<typename... Ts>
	struct checker {
		constexpr int operator () (one<T> v) {
			static_assert(count<T, Ts...>() == 1);
			return 1;
		}
	};
};

template<typename T>
struct multiple {
	struct params_requirement_mark{};

	template<typename... Ts>
	struct checker {
		constexpr int operator () (multiple<T> v) {
			constexpr int c = count<T, Ts...>();
			static_assert(c >= 1);
			return c;
		}
	};
};

template<typename T>
struct optional {
	struct params_requirement_mark{};

	template<typename... Ts>
	struct checker {
		constexpr int operator () (optional<T> v) {
			constexpr int c = count<T, Ts...>();
			static_assert(c <= 1);
			return c;
		}
	};
};

template<typename T>
concept params_requirement = requires{
	typename T::params_requirement_mark;
};

template<typename T>
concept non_params_requirement = !params_requirement<T>;

template<params_requirement... Infos>
struct wrapper {
	template<non_params_requirement... Ps>
	static constexpr bool check() {
		int used = 0;

		u::for_each(
			Infos{}...,
			[&](auto v) {
				used += u::do_one_of{ typename Infos::template checker<Ps...>{} ... } (v);
			}
		);

		if(used != sizeof...(Ps)) return false;
		return true;
	}
};

template<params_requirement... Infos>
auto make_params(auto... ps) {
	static_assert(wrapper<Infos...>::template check<decltype(ps)...>());
	return params<decltype(ps)...>{ ps... };
}


} // u