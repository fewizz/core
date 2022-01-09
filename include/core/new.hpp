#include "../core/integer.hpp"

[[nodiscard]] inline void* operator new (nuint size, void* ptr ) noexcept {
	return ptr;
}