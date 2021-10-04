#pragma once

#include "indices_of_ranges_of_value_type.hpp"

namespace types {

	template<typename ValueType>
	struct count_of_ranges_of_value_type {

		template<typename... Types>
		static constexpr uint for_types_of =
			types::indices_of_ranges_of_value_type<
				ValueType
			>::
			template for_types_of<
				Types...
			>::size;

	};

}