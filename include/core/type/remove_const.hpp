#pragma once

#include "of.hpp"

namespace type {

	class remove_const {
		template<typename Type>
		struct remove_const_t : type::of<Type>{};

		template<typename Type>
		struct remove_const_t<const Type> : type::of<Type>{};
	public:

		template<typename Type>
		using for_type = typename remove_const_t<Type>::type;

		static constexpr bool is_type_modifier = true;
	};

}