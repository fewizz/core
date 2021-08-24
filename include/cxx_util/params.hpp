#pragma once

#include <tuple>
#include <optional>
#include <type_traits>
#include <utility>
#include "tuple/for_each.hpp"
#include "tuple/get.hpp"
#include "parameter_pack/parameter_pack.hpp"
#include "named.hpp"

namespace u {

struct required {
	template<std::size_t Calls>
	constexpr static void check() {
		static_assert(Calls == 1);
	}
};

struct several {
	template<std::size_t Calls>
	constexpr static void check() {
		static_assert(Calls >= 1);
	}
};

struct optional {
	template<std::size_t Calls>
	constexpr static void check() {
		static_assert(Calls <= 1);
	}
};

struct any {
	template<std::size_t Calls>
	constexpr static void check() {}
};

template<typename... Ts>
struct params : std::tuple<Ts...> {

	using std::tuple<Ts...>::tuple;

	std::tuple<Ts...>& tuple() {
		return (std::tuple<Ts...>&) *this;
	}

	const std::tuple<Ts...>& tuple() const {
		return (const std::tuple<Ts...>&) *this;
	}

	params(Ts&&... ts) : std::tuple<Ts...>{ std::forward<Ts>(ts)... } {}
	params(std::tuple<Ts...> t) : std::tuple<Ts...>{ std::move(t) } {}

protected:
	template<typename Req, typename F, std::size_t Index = 0, std::size_t... NotCalledIndices>
	auto handle0(F&& f, std::index_sequence<NotCalledIndices...> nci = {}) {
		if constexpr(Index == sizeof...(Ts)) {
			Req::template check<sizeof...(Ts) - sizeof...(NotCalledIndices)>();
			
			return u::params {
				u::get(std::move(tuple()), nci)
			};
		}
		else {
			using T = std::tuple_element_t<Index, std::tuple<Ts...>>;

			if constexpr(std::is_invocable_v<F, T>) {
				f(std::forward<T>(std::get<Index>(tuple())));
				return handle0<Req, F, Index+1, NotCalledIndices...>(std::forward<F>(f));
			}
			else {
				return handle0<Req, F, Index+1, NotCalledIndices..., Index>(std::forward<F>(f));
			}
		}
	}

public:
	template<typename Req, typename F>
	auto handle(F&& f) {
		return handle0<Req, F, 0>(std::forward<F>(f));
	}

	constexpr auto& check_for_emptiness() const {
		static_assert(sizeof...(Ts) == 0);
		return *this;
	}

	template<typename T>
	static constexpr std::size_t count = u::count<T, Ts...>;

	template<typename T>
	static constexpr bool contains = count<T> > 0;
};

template<typename... Ts>
params(Ts&&...) -> params<Ts...>;

template<typename... Ts>
params(std::tuple<Ts...>) -> params<Ts...>;

} // u