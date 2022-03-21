#pragma once

#include "../meta/type/remove_const.hpp"
#include "../meta/type/is_same_as.hpp"
#include "../meta/type/remove_reference.hpp"

namespace range {

	template<typename T>
	concept basic = requires(T t) {
		t.begin();
		t.end();
		typename remove_reference<remove_const<T>>::value_type;
	};

} // range