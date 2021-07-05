#pragma once

#include <cstddef>
#include <limits>
#include <pthread.h>
#include <type_traits>
#include "parameter_pack/at.hpp"
#include "parameter_pack/index_of.hpp"

namespace u {

template<class... Ts>
struct parameter_pack;

namespace internal {

template<class... Ts>
struct parameter_pack_base {

	template<class T>
	struct push_front { using type = parameter_pack<T, Ts...>; };

	template<class T>
	using push_front_t = typename push_front<T>::type;

	template<class T>
	struct push_back { using type = parameter_pack<Ts..., T>; };

	template<class T>
	using push_back_t = typename push_back<T>::type;
	
	template<std::size_t begin, std::size_t end, std::size_t = size>
	struct cut {
		static_assert(end - begin <= size, "range of bounds");
		static_assert(end - begin >= 0, "size of new type vector is negative");

		using type = typename internal::cut<begin, end, parameter_pack<>, Ts...>::type;
	};

	template<std::size_t beginIn, std::size_t endEx>
	struct cut<beginIn, endEx, 0> {
		static_assert(beginIn == 0 && endEx == 0, "range out of bounds");
		using type = parameter_pack<>;
	};

	template<std::size_t beginIn, std::size_t endEx>
	using cut_t = typename cut<beginIn, endEx>::type;
	
	template<std::size_t endEx>
	struct cut_until {
		using type = cut_t<0, endEx>;
	};

	template<std::size_t endEx>
	using cut_until_t = typename cut_until<endEx>::type;

	template<std::size_t beginIn>
	struct cut_from {
		using type = cut_t<beginIn, sizeof...(Ts)>;
	};

	template<std::size_t beginIn>
	using cut_from_t = typename cut_from<beginIn>::type;

	template<class... Ts0> struct append {
		using type = parameter_pack<Ts..., Ts0...>;
	};
	
	template<class... Ts0>
	struct append<parameter_pack<Ts0...>> {
		using type = parameter_pack<Ts..., Ts0...>;
	};

	template<class... Ts0>
	using append_t = typename append<Ts0...>::type;
	
	template<std::size_t index, class...Unused>
	struct erase {
		static_assert(index >= 0 and index < size, "index out of bounds");
		using type = typename cut_until_t<index>::template append_t<cut_from_t<index+1>>;
	};

	template<std::size_t index>
	using erase_t = typename erase<index>::type;
	
	template<class T>
	struct erase_first {
		static_assert(contains_v<T>, "no such type");
		using type = erase_t<index_of<T>>;
	};

	template<class T>
	using erase_first_t = typename erase_first<T>::type;
	
	template<class...Unused>
	struct erase_front {
		using type = erase_t<0>;
	};
	
	template<class...Unused>
	using erase_front_t = typename erase_front<Unused...>::type;
	
	template<class...Unused>
	struct erase_back {
		using type = erase_t<size-1>;
	};
	
	template<class...Unused>
	using erase_back_t = typename erase_back<Unused...>::type;
};

}

template<typename... Ts> 
struct parameter_pack;

template<>
struct parameter_pack<> {
	static constexpr std::size_t size = 0;
	static constexpr bool empty = true;

	template<class T>
	static constexpr std::size_t index_of = u::not_found;

	template<class T>
	using append_back = parameter_pack<T>;
};

namespace internal {
	// from
	template<std::size_t Index, typename... Ts>
	struct from;

	template<std::size_t Index, typename T, typename... Ts>
	struct from<Index, T, Ts...> {
		using type = typename from<Index - 1, Ts...>::type;
	};

	template<typename... Ts>
	struct from<0, Ts...> {
		using type = parameter_pack<Ts...>;
	};

	// until
	template<typename... Result>
	struct holder {
		template<std::size_t Index, typename... Ts>
		struct until;

		template<std::size_t Index, typename T, typename... Ts>
		struct until<Index, T, Ts...> {
			using type = typename holder<Result..., T>::template until<Index - 1, Ts...>::type;
		};

		template<typename... Ts>
		struct until<0, Ts...> {
			using type = parameter_pack<Result...>;
		};
	};
}

template<typename... Ts>
struct parameter_pack {
	static constexpr std::size_t size = sizeof...(Ts);
	static constexpr bool empty = false;

	template<std::size_t index>
	using at = typename u::at<index, Ts...>;

	using front = at<0>;
	using back = at<size - 1>;

	template<typename T>
	static constexpr std::size_t index_of = u::index_of<T, Ts...>;

	template<typename T>
	static constexpr bool contains = index_of<T> != not_found;

	template<typename... Ts0>
	using append_back = parameter_pack<Ts..., Ts0...>;

	template<typename... Ts0>
	using append_front = parameter_pack<Ts0..., Ts...>;

	template<std::size_t Index>
	using until = typename internal::holder<>::until<Index, Ts...>::type;

};

}