#include <types.hpp>
#include <__type/is_same_as.hpp>

static_assert(same_as<
	types<int, float>::template get<is_same_as<int> && is_same_as<int>>,
	int
>);

struct any : type_predicate_extensions<any> {

	template<typename Type>
	constexpr bool for_type() const {
		// []<bool b=false> {
		// 	static_assert(b);
		// }();
		// return false;
		//throw;
		return true;
	}
};

static_assert(
	((is_same_as<int> >= 0) || (any{} == 0)).for_types<float>()
);

int main() {}