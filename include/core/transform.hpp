#pragma once

#include "range.hpp"
#include "forward.hpp"
#include "value_type.hpp"
#include "meta/type/decay.hpp"
#include "zip.hpp"

template<typename Function, typename... Iterators>
                                  // signing for problems
class transformed_view_iterator : public zip_view_iterator<Iterators...> {
	Function& function_;
	using base_type = zip_view_iterator<Iterators...>;
public:

	constexpr transformed_view_iterator(
		Function& function, base_type base
	) :
		base_type{ base },
		function_{ function }
	{}

	constexpr decltype(auto) operator * () {
		return (base_type::operator * ())
			.pass([&]<typename... Types>(Types&&... elements) {
				return function_(forward<Types>(elements)...);
			});
	}

	constexpr auto& operator ++ () {
		base_type::operator ++ ();
		return *this;
	}

	constexpr auto& operator += (nuint n) {
		base_type::operator += (n);
		return *this;
	}

	constexpr auto operator + (nuint n) {
		transformed_view_iterator cpy{ *this };
		return cpy += n;
	}

};

template<typename Function, range... Ranges>
class transform_view : zip_view<Ranges...> {
	Function function_;
	using base_type = zip_view<Ranges...>;

	// TODO simplify
	template<typename... Args>
	constexpr transform_view(elements::of<Args...> args) :
		base_type {
			[&]<nuint... Indices>(indices::of<Indices...>) {
				return elements::of{ args.template forward<Indices>()... };
			}(indices::from<0>::to<sizeof...(Args) - 1>{})
		},
		function_ {
			args.template forward<sizeof...(Args) - 1>()
		}
	{}

public:

	template<typename... Args>
	constexpr transform_view(Args&&... args) :
		transform_view(elements::of{ forward<Args>(args)... })
	{}

	constexpr auto begin() const {
		return transformed_view_iterator {
			function_, base_type::begin()
		};
	}

	using base_type::end;

	constexpr auto operator [] (nuint n) const {
		return *(begin() + n);
	}

};

// TODO
template<range Range, typename Function>
transform_view(Range&&, Function&&)
	-> transform_view<Function, Range>;

template<range Range0, range Range1, typename Function>
transform_view(Range0&&, Range1&&, Function&&)
	-> transform_view<Function, Range0, Range1>;