#pragma once

template<typename Type, typename WhatToAddAssign>
concept addition_assignable = requires(Type t, WhatToAddAssign w) {
	t += w;
};