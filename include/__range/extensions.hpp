#pragma once

#include "./extensions_declaration.hpp"
#include "./sized.hpp"
#include "./element_index_type.hpp"

template<typename Derived, range_extensions_options Options>
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

	constexpr basic_iterator auto _iterator() const {
		return range_iterator(range_());
	}
	constexpr basic_iterator auto _iterator()       {
		return range_iterator(range_());
	}
	constexpr auto _sentinel() const { return range_sentinel(range_()); }
	constexpr auto _sentinel()       { return range_sentinel(range_()); }

public:

	// for compatibility
	constexpr basic_iterator auto begin() const { return _iterator(); }
	constexpr basic_iterator auto begin()       { return _iterator(); }
	constexpr auto end() const { return _sentinel(); }
	constexpr auto end()       { return _sentinel(); }

	template<typename Handler>
	void for_each_indexed(Handler handler)       & {
		using index_type = range_element_index_type<Derived>;
		index_type i{};
		for(auto& e : range_()) {
			handler(e, i); ++i;
		}
	}
	template<typename Handler>
	void for_each_indexed(Handler handler) const & {
		using index_type = range_element_index_type<Derived>;
		index_type i{};
		for(auto& e : range_()) {
			handler(e, i); ++i;
		}
	}

	template<typename Handler>
	void for_each_index(Handler handler) const {
		using index_type = range_element_index_type<Derived>;
		auto size = range_size(range_());
		for(index_type i{}; i < size; ++i) {
			handler(i);
		}
	}

	template<typename Element>
	constexpr bool contains(Element&& element) const;

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const &;

	template<basic_range OtherRange>
	constexpr bool has_equal_size_and_elements(
		OtherRange&& other_range
	) const;

	constexpr auto flat_view() const &  ;
	constexpr auto flat_view()       &  ;
	constexpr auto flat_view() const && ;
	constexpr auto flat_view()       && ;

	constexpr auto dereference_view() const &  ;
	constexpr auto dereference_view()       &  ;
	constexpr auto dereference_view() const && ;
	constexpr auto dereference_view()       && ;

	constexpr auto reverse_view() const &  ;
	constexpr auto reverse_view()       &  ;
	constexpr auto reverse_view() const && ;
	constexpr auto reverse_view()       && ;

	constexpr auto sized_view() const &  ;
	constexpr auto sized_view()       &  ;
	constexpr auto sized_view() const && ;
	constexpr auto sized_view()       && ;

	template<typename Predicate>
	constexpr auto filter_view(Predicate&&) const &  ;
	template<typename Predicate>
	constexpr auto filter_view(Predicate&&)       &  ;
	template<typename Predicate>
	constexpr auto filter_view(Predicate&&) const && ;
	template<typename Predicate>
	constexpr auto filter_view(Predicate&&)       && ;

	template<typename Predicate>
	constexpr auto transform_view(Predicate&&) const &  ;
	template<typename Predicate>
	constexpr auto transform_view(Predicate&&)       &  ;
	template<typename Predicate>
	constexpr auto transform_view(Predicate&&) const && ;
	template<typename Predicate>
	constexpr auto transform_view(Predicate&&)       && ;

	constexpr decltype(auto) operator [] (nuint index) const {
		return *(_iterator() + index);
	}

	constexpr decltype(auto) operator [] (nuint index) {
		return *(_iterator() + index);
	}

	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler) const & ;
	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler)       & ;
	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler) const &&;
	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(Handler&& handler)       &&;

	template<typename Predicate>
	auto try_find_first_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_last_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_index_of_first_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_index_of_last_satisfying(Predicate&& predicate) const;

	auto shrink_view(auto size) const;

	template<typename... With>
	bool starts_with(With&&... with) const &;

	template<typename... With>
	bool ends_with(With&&... with) const &;

	nuint get_or_compute_size() const;

};