#pragma once

enum class endianness {
	little, big,
	#if __LITTLE_ENDIAN__
	native = little
	#elif __BIG_ENDIAN__
	native = big
	#else
	???
	#endif
};