#pragma once

template<typename BaseType, typename Type>
concept base_of = __is_base_of(BaseType, Type);