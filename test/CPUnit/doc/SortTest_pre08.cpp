#include <unittest>
#include <algorithm>
#include <vector>

namespace SortTest {
        
  using namespace unittest;

  const int length = 3;
  const int data[length] = {3, 1, 2};
  std::vector<int> input;
          
  UNITTEST_SET_UP(SortTest) {
    input = std::vector<int>(data, data + length);
  }

  UNITTEST_TEAR_DOWN(SortTest) {
    input.clear();
  }

  UNITTEST_FUNC(SortTest, test_sort) {
    // Construct the expected result
    const int expected_data[length] = {1, 2, 3};
    std::vector<int> expected(expected_data, expected_data + length);

    // Sort and then test the actual result
    std::sort(input.begin(), input.end());
    assert_equals("std::sort failed.", expected, input);
  }

  // Construct a reverse comparator for the next test.
  struct RevCmp {
    bool operator () (int i1, int i2) {
      return i2 < i1;
    }
  };
  
  UNITTEST_FUNC(SortTest, test_reverse_sort) {

    // Construct the expected result
    const int expected_data[length] = {2, 3, 1};
    std::vector<int> expected(expected_data, expected_data + length);

    // Sort and then test the actual result
    std::sort(input.begin(), input.end(), RevCmp());
    assert_equals("std::sort reversely failed.", expected, input);
  }
}
