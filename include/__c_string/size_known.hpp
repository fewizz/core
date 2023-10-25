#pragma once

#include "../span.hpp"
#include "./declaration.hpp"

template<typename Type, typename SizeType>
struct c_string<c_string_type::known_size, Type, SizeType> :
	span<const Type, SizeType>
{
private:
	using base_type = span<const Type, SizeType>;
public:

	c_string() = default;

	template<contiguous_range Range>
	constexpr c_string(Range&& range) : base_type{ forward<Range>(range) } {}

	template<nuint Size>
	constexpr c_string(const Type (&array)[Size]) :
		base_type{ array, Size - 1 }
	{}

	constexpr c_string(const Type* ptr, SizeType size) :
		base_type{ ptr, size }
	{}

	using base_type::operator = ;
};

template<typename Type, nuint Size>
c_string(const Type(&)[Size]) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;

template<typename Type>
c_string(const Type*, nuint size) -> c_string<
	c_string_type::known_size,
	remove_const<Type>
>;


#include "../__range/view_copied_elements_on_stack.hpp"
#include "../__range/copy.hpp"

template<basic_range Range, typename Handler>
decltype(auto) view_on_stack_as_c_string(Range&& range, Handler&& handler) {
	auto size = range_size(range);
	return view_on_stack<char>{size + 1}(
		[&](span<char> span) -> decltype(auto) {
			__range::copy{ range }.to(span);
			span[size] = 0;
			return handler(c_string_of_known_size<char>{span.iterator(), size});
		}
	);
}