#pragma once

template<typename T>
concept range = requires(T t) {
	t.begin();
	t.end();
	T::value_type;
};