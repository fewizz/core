#pragma once

#include "basic.hpp"
#include "../forward.hpp"

namespace range {

	template<typename It, typename F>
	struct transformed_iterator {
		using base_iterator_type = It;

		It base_iterator;
		F& tansform_function;

		constexpr transformed_iterator(It it, F& f) : base_iterator{ it }, tansform_function{ f } {}

		constexpr decltype(auto) operator * () const {
			return tansform_function(*base_iterator);
		}

		constexpr auto& operator ++ () {
			++base_iterator;
			return *this;
		}

		constexpr bool operator == (transformed_iterator other) const {
			return base_iterator == other.base_iterator;
		}
	};

	template<range::basic R, typename F>
	struct transformed {
		R&& base_range;
		F&& tansform_function;

		using value_type = type::remove_reference::for_type<decltype(tansform_function(*base_range.begin()))>;

		transformed(R&& r, F&& f) : base_range{ forward<R>(r) }, tansform_function{ forward<F>(f) } {}

		auto begin() const {
			return transformed_iterator{ base_range.begin(), tansform_function };
		}

		auto end() const {
			return transformed_iterator{ base_range.end(), tansform_function };
		}
	};

	template<range::basic R>
	struct transform {
		R&& range;

		transform(R&& r) : range{ forward<R>(r) } {}

		template<typename F>
		auto operator () (F&& f) const {
			return transformed<R, F>(forward<R>(range), forward<F>(f));
		}
	};

	template<range::basic R>
	transform(R&&) -> transform<R>;

}