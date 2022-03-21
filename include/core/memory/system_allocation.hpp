#pragma once

#include "../span.hpp"
#include "../exchange.hpp"

struct system_memory_allocation : span<uint8> {

	system_memory_allocation() = default;

	system_memory_allocation(nuint size) :
		span<uint8>{ new uint8[size], size } 
	{}

	system_memory_allocation(system_memory_allocation&& other) :
		span<uint8>{ exchange(other.m_values, nullptr), exchange(other.m_size, 0) }
	{}

	~system_memory_allocation() {
		if(data() != nullptr) {
			delete[] data();
		}
	}

};