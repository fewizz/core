#include "parameter_pack/parameter_pack.hpp"
#include <type_traits>

int main() {
	using namespace std;
	using IFB = u::parameter_pack<int, float, bool>;

	static_assert(!IFB::empty);
	static_assert(IFB::size == 3);

	static_assert(is_same_v<IFB::at<0>, int>);
	static_assert(is_same_v<IFB::at<1>, float>);
	static_assert(is_same_v<IFB::at<2>, bool>);

	static_assert(IFB::index_of<int> == 0);
	static_assert(IFB::index_of<float> == 1);
	static_assert(IFB::index_of<bool> == 2);

	static_assert(is_same_v<IFB::front, int>);
	static_assert(is_same_v<IFB::back, bool>);

	static_assert(IFB::contains<int>);
	static_assert(IFB::contains<float>);
	static_assert(IFB::contains<bool>);
	static_assert(!IFB::contains<double>);

	using DIFB = IFB::append_front<double>;
	static_assert(DIFB::size == 4);
	static_assert(is_same_v<DIFB::front, double>);

	using IFBD = IFB::append_back<double>;
	static_assert(IFBD::size == 4);
	static_assert(is_same_v<IFBD::back, double>);

	static_assert(is_same_v<IFB::from<0>, u::parameter_pack<int, float, bool>>);
	static_assert(is_same_v<IFB::from<1>, u::parameter_pack<float, bool>>);
	static_assert(is_same_v<IFB::from<2>, u::parameter_pack<bool>>);
	static_assert(IFB::from<3>::empty);

	static_assert(IFB::until<0>::empty);
	static_assert(is_same_v<IFB::until<1>, u::parameter_pack<int>>);
	static_assert(is_same_v<IFB::until<2>, u::parameter_pack<int, float>>);
	static_assert(is_same_v<IFB::until<3>, u::parameter_pack<int, float, bool>>);
}