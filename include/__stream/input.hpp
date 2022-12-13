#pragma once

#include <type.hpp>

struct input_stream_mark {};

template<typename Type>
concept input_stream = base_of<input_stream_mark, Type>;