#pragma once

#include "./extensions_declaration.hpp"
#include "./sized.hpp"

template<typename Derived>
struct range_extensions {

private:
	constexpr const Derived&
	range_() const &  { return (const Derived&)  *this; }
	constexpr       Derived&
	range_()       &  { return (      Derived&)  *this; }
	constexpr const Derived&&
	range_() const && { return (const Derived&&) *this; }
	constexpr       Derived&&
	range_()       && { return (      Derived&&) *this; }
public:

	constexpr basic_iterator auto iterator() const {
		return range_iterator(range_());
	}
	constexpr basic_iterator auto iterator()       {
		return range_iterator(range_());
	}

	constexpr auto sentinel() const { return range_sentinel(range_()); }
	constexpr auto sentinel()       { return range_sentinel(range_()); }

	template<typename Element>
	constexpr bool contains(Element&& element) const;

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const;

	template<basic_range OtherRange>
	constexpr bool have_elements_equal_to(
		OtherRange&& other_range
	) const;

	constexpr auto flat_view() const &  ;
	constexpr auto flat_view()       &  ;
	constexpr auto flat_view() const && ;
	constexpr auto flat_view()       && ;

	constexpr auto size() const {
		static_assert(sized_range<Derived>);
		return sentinel() - iterator();
	}

	constexpr decltype(auto) operator [] (nuint index) const {
		return *(iterator() + index);
	}

	constexpr decltype(auto) operator [] (nuint index) {
		return *(iterator() + index);
	}

	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler) const;

	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler);

	template<typename Predicate>
	auto try_find_first_satisfying(Predicate&& predicate) const;

};