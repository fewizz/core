#pragma once

#include "./__range/basic.hpp"
#include "./__ranges/have_equal_size_and_elements.hpp"
#include "./__ranges/concat_view.hpp"
#include "./__ranges/transform_view.hpp"
#include "./tuple.hpp"
#include "./forward.hpp"

template<basic_range... Ranges>
struct ranges {
private:
	tuple<Ranges...> ranges_;
public:

	constexpr ranges(Ranges&&... ranges) :
		ranges_{ ::forward<Ranges>(ranges)... }
	{}

	constexpr bool have_equal_size_and_elements() const {
		return ranges_.pass([](const Ranges&... ranges) {
			return __ranges::have_equal_size_and_elements(ranges...);
		});
	}

	template<typename Self>
	constexpr auto concat_view(this Self&& self) {
		return (::forward<Self>(self).ranges_)
			.pass([]<typename... _Ranges>(_Ranges&&... ranges) {
				return __ranges::concat_view{ forward<_Ranges>(ranges)... };
			});
	}
	
	template<typename Function>
	constexpr auto transform_view(Function&& function) const & {
		return ranges_.pass([&](const Ranges&... ranges) {
			return __ranges::transform_view_t {
				forward<Function>(function), ranges...
			};
		});
	}
	template<typename Function>
	constexpr auto transform_view(Function&& function)       & {
		return ranges_.pass([&](Ranges&... ranges) {
			return __ranges::transform_view_t {
				forward<Function>(function), ranges...
			};
		});
	}

	template<typename Function>
	constexpr auto transform_view(Function&& function) const && {
		return ranges_.forward([&](const Ranges&&... ranges) {
			return __ranges::transform_view_t {
				forward<Function>(function), forward<Ranges>(ranges)...
			};
		});
	}
	template<typename Function>
	constexpr auto transform_view(Function&& function)       && {
		return ranges_.forward([&](Ranges&&... ranges) {
			return __ranges::transform_view_t {
				forward<Function>(function), forward<Ranges>(ranges)...
			};
		});
	}

};

template<basic_range... Ranges>
ranges(Ranges&&...) -> ranges<Ranges...>;