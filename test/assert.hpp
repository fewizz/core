#pragma once

extern "C" void abort();

inline void assert(bool b) {
	if(!b) {
		abort();
	}
}