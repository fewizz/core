#pragma once

#include "../meta/type/is_same_as.hpp"
#include "../meta/type/remove_reference.hpp"

namespace range {

	template<typename T>
	concept basic = requires(T t) {
		t.begin();
		t.end();
		typename type::remove_reference::for_type<T>::value_type;
	};
}