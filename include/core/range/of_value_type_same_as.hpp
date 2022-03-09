#pragma once

#include "of_value_type_satisfying_predicate.hpp"

namespace range {

	template<typename Range, typename Value>
	concept of_value_type_same_as = range::of_value_type_satisfying_predicate<Range, type::is_same_as<Value>>;

	// aliases

	template<typename Range, typename Value>
	concept of_value_type = of_value_type_same_as<Range, Value>;

	template<typename Range, typename Value>
	concept of = of_value_type_same_as<Range, Value>;

}

namespace type {

	template<typename Value>
	struct is_range_of_value_type_same_as : type::predicate_marker {

		template<typename Range>
		static constexpr bool for_type = range::of_value_type_same_as<Range, Value>;

	};

	// aliases

	template<typename Value>
	struct is_range_of_value_type : type::is_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct is_range_of : type::is_range_of_value_type_same_as<Value> {};

}

namespace types {

	template<typename Value>
	struct count_of_ranges_of_value_type_same_as : types::count_of_ranges_of_value_type_satisfying_predicate<type::is_same_as<Value>> {};

	template<typename Value>
	struct are_contain_range_of_value_type_same_as : types::are_contain_range_of_value_type_satisfying_predicate<type::is_same_as<Value>> {};

	template<typename Value>
	struct are_may_contain_range_of_value_type_same_as : types::are_may_contain_range_of_value_type_satisfying_predicate<type::is_same_as<Value>> {};

	template<typename Value>
	struct are_contain_one_range_of_value_type_same_as : types::are_contain_one_range_of_value_type_satisfying_predicate<type::is_same_as<Value>> {};

	template<typename Value>
	struct are_may_contain_one_range_of_value_type_same_as : types::are_may_contain_one_range_of_value_type_satisfying_predicate<type::is_same_as<Value>> {};

	// aliases
	
	template<typename Value>
	struct count_of_ranges_of_value_type : types::count_of_ranges_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_contain_range_of_value_type : types::are_contain_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_may_contain_range_of_value_type : types::are_may_contain_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_contain_one_range_of_value_type : types::are_contain_one_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_may_contain_one_range_of_value_type : types::are_may_contain_one_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct count_of_ranges_of : types::count_of_ranges_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_contain_range_of : types::are_contain_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_may_contain_range_of : types::are_may_contain_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_contain_one_range_of : types::are_contain_one_range_of_value_type_same_as<Value> {};

	template<typename Value>
	struct are_may_contain_one_range_of : types::are_may_contain_one_range_of_value_type_same_as<Value> {};

}

namespace elements {

	template<typename Value>
	inline constexpr auto range_of_value_type_same_as = elements::range_of_value_type_satisfying_predicate<type::is_same_as<Value>>;

	// aliases

	template<typename Value>
	inline constexpr auto range_of_value_type = elements::range_of_value_type_same_as<Value>;

	template<typename Value>
	inline constexpr auto range_of = elements::range_of_value_type_same_as<Value>;

}