#pragma once

#include "./integer.hpp"

constexpr inline void* operator new (nuint, void* ptr) { return ptr; }