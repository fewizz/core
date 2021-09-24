#pragma once

template<typename T>
concept enum_type = __is_enum(T);