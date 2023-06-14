#pragma once

template<typename Type>
concept growable_range = requires(Type range) {
	range.grow();
};