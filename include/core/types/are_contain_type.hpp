#pragma once

#include "indices_of_type.hpp"
#include "at_indices.hpp"
#include "../type/remove_reference.hpp"
#include "../type/remove_const.hpp"
#include "../type/decay.hpp"
#include "../types/modified_predicate.hpp"

namespace types {

	template<typename Type>
	struct are_contain_type : types::predicate_marker {

		template<typename... Types>
		using indices_of_affected_types_of = typename indices_of_type<Type>::template for_types_of<Types...>;

		template<typename... Types>
		static constexpr bool for_types_of = ! indices_of_affected_types_of<Types...>::is_empty;

		//using ignore_reference = types::modified_predicate<types::are_contain_type<Type>, type::remove_reference>;
		//using ignore_const = types::modified_predicate<types::are_contain_type<Type>, type::remove_const>;
		//using decay = types::modified_predicate<types::are_contain_type<Type>, type::decay>;
	};

}