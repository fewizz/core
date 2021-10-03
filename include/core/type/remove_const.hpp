#pragma once

#include "of.hpp"

namespace type {

	class remove_const {
		template<typename Type>
		struct remove_const_t : type::of<Type>{};

		template<typename T>
		struct remove_const_t<const T> : type::of<T>{};
	public:

		template<typename Type>
		using for_type_of = typename remove_const_t<Type>::type;
	};

}