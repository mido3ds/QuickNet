
#include <unittest>

using namespace unittest;

UNITTEST_GFUNC(example_test) {
  const int i1 = 33;
  const int i2 = i1;
  assert_equals("The two integers should be equal.", i1, i2);
}

