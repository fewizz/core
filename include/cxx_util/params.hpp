#pragma once

#include <tuple>
#include <optional>
#include "tuple/for_each.hpp"
#include "tuple/get.hpp"
#include "parameter_pack/indices_of.hpp"
#include "parameter_pack/indices_of_invocable_with.hpp"
#include "named.hpp"

namespace u {

template<typename T>
struct required : u::named<T> {
	struct param_requirement_mark{};

	template<typename... Ts>
	struct checker {
		constexpr int operator () (required<T> v) {
			static_assert(count<T, Ts...>() == 1);
			return 1;
		}
	};
};

template<typename T>
struct multiple {
	struct param_requirement_mark{};

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
	struct param_requirement_mark{};

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
concept param_requirement = requires{
	typename T::param_requirement_mark;
};

template<typename T>
concept non_param_requirement = !param_requirement<T>;

template<param_requirement... Reqs>
struct for_param_requirements {

	template<non_param_requirement... Ps>
	static constexpr bool check_usage() {
		int used = 0;

		u::for_each(
			Reqs{}...,
			[&](auto v) {
				used += u::do_one_of{ typename Reqs::template checker<Ps...>{} ... } (v);
			}
		);

		if(used != sizeof...(Ps)) return false;
		return true;
	}
};

template<typename T, typename... Fs>
bool check_param() {
	constexpr auto idx = u::indices_of_invocable_with<T, Fs...>{};
	static_assert(idx.size() <= 1);
	return idx.size();
}

template<typename... Ps>
auto parse(std::tuple<Ps&&...> params, auto&&... fs) {

}

} // u