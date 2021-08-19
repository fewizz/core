#pragma once

#include <tuple>
#include <optional>
#include "tuple/for_each.hpp"
#include "tuple/get.hpp"
#include "parameter_pack/indices_of.hpp"

namespace u {

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

template<params_requirement... Reqs>
struct for_params_requirements {

	template<non_params_requirement... Ps>
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

} // u