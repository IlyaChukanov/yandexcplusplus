#include "../../../MyUtils/MyTestFramework/TestFramework.h"
#include <functional>
#include <string>
using namespace std;

template <typename T>
class LazyValue {
public:
  explicit LazyValue(std::function<T()> init) : init_func_(std::move(init)) {}

  bool HasValue() const {
    return value_.has_value();
  }
  const T& Get() const {
    if (!value_.has_value()) {
      value_ = init_func_();
    }
    return value_.value();
  }

private:
  mutable std::optional<T> value_;
  std::function<T()> init_func_;
};

void UseExample() {
  const string big_string = "Giant amounts of memory";

  LazyValue<string> lazy_string([&big_string] { return big_string; });

  ASSERT(!lazy_string.HasValue());
  ASSERT_EQUAL(lazy_string.Get(), big_string);
  ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
  bool called = false;

  {
    LazyValue<int> lazy_int([&called] {
      called = true;
      return 0;
    });
  }
  ASSERT(!called);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, UseExample);
  RUN_TEST(tr, TestInitializerIsntCalled);
  return 0;
}
