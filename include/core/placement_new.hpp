#pragma once

#include "integer.hpp"

inline void* operator new (nuint, void* ptr) { return ptr; }