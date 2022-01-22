#pragma once

#include "is_same_as.hpp"
#include "decay.hpp"

namespace type {

	template<typename BaseType>
	struct is_decayed_same_as : type::predicate_marker {

		template<typename TypeToDecay>
		static constexpr bool for_type = type::is_same_as<BaseType>::template for_type<::decay<TypeToDecay>>;

	};

}