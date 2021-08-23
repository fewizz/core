#pragma once

#include <tuple>
#include "../parameter_pack/parameter_pack.hpp"
#include "get.hpp"

namespace u {

template<typename Erd, typename Rem>
class erase_result : std::pair<Erd, Rem> {

public:
	
	constexpr erase_result(Erd e, Rem r)
		: std::pair<Erd, Rem>{e, r} {}

	constexpr auto& remaining() const {
		return this->second;
	}

	constexpr Erd& erased() const {
		return ((std::pair<Erd, Rem>*)this)->first;
	}

	constexpr auto size() const {
		return std::tuple_size_v<Erd>;
	}

	constexpr bool has_value() const {
		return size() > 0;
	}

	constexpr auto value() const {
		return std::get<0>(erased());
	}
};

template<typename T, typename... Ts>
constexpr auto erase(std::tuple<Ts...> t) {
	using PP = typename u::parameter_pack<Ts...>;
	return erase_result {
		u::get<Ts...>(std::move(t), typename PP::template indices_of_same_as<T>{}),
		u::get<Ts...>(std::move(t), typename PP::template indices_of_not_same_as<T>{})
	};
}

template<typename T, typename... Ts>
constexpr auto erase(Ts... t) {
	return erase<T, Ts...>(std::forward_as_tuple(t...));
}

}