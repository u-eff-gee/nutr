#include <cassert>
#include <cmath>

using std::fabs;

template<typename T>
void test_numerical_equality(const T a, const T b, const T epsilon) {
	assert(fabs(a-b) < fabs(epsilon));
};

