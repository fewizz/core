#pragma once

#include <array.hpp>
#include <range.hpp>

bool consteval f() {
	array a{ 0.0F, 0.1F, 0.2F };
	if(!range{ a }.contains( 0.2F )) throw;


	array<float, 3> empty;
	range{ a }.copy_to(empty);
	if(empty[0] != 0.0F || empty[1] != 0.1F || empty[2] != 0.2F) throw;

	

	return true;
}

static_assert(f());