#pragma once

struct type_modifier_marker {};

template<typename Type>
concept type_modifier = __is_base_of(type_modifier_marker, Type);