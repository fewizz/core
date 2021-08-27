#pragma once

#include <__utility/integer_sequence.h>
#include <cstddef>
#include <utility>

namespace u {

namespace internal {

	/*template<std::size_t Index, std::size_t CurrentIndex = 0>
	struct erase_at {

		template<std::size_t... Left>
		struct left {

			template<std::size_t Current, std::size_t... Right>
			struct right {
				using type
					= typename erase_at<Index, CurrentIndex + 1>
						::template left<Left..., Current>
						::template right<Right...>;
			};

			template<std::size_t Current, std::size_t... Right>
			requires(CurrentIndex == Index)
			struct right<Current, Right...> {
				using type = std::index_sequence<Left..., Right...>;
			};
		};
	};*/

	namespace erase_at {

		template<std::size_t... Indices>
		struct indices {

			template<typename T, T... Value>
			struct values {

				template<std::size_t Index = 0>
				struct result {
					using type = 
				};

			};
		};
	}
}

/*template<std::size_t Index, std::size_t... Indices>
using erase_at_t
	= typename internal::template erase_at<Index>
		::template left<>
		::template right<Indices...>;*/

template<std::size_t... Indices, typename T, T... Values>
auto erase_at(std::index_sequence<Indices...>, std::integer_sequence<T, Values...>) {
	return erase_at_t<Index, Indices...>{};
}

}