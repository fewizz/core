#pragma once

#include "of.hpp"
#include "modifier.hpp"

namespace type {
	
	class remove_reference {
		template<typename Type>
		struct remove_reference_t : type::of<Type> {};

		template<typename Type>
		struct remove_reference_t<Type&> : type::of<Type> {};

		template<typename Type>
		struct remove_reference_t<Type&&> : type::of<Type> {};
	public:

		template<typename Type>
		using for_type_of = typename remove_reference_t<Type>::type;

		static constexpr bool is_type_modifier = true;

	};

}