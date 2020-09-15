
#define CATCH_CONFIG_MAIN
#include <thread>

#include "Catch2/single_include/catch2/catch.hpp"
#include "locked.hpp"

TEST_CASE("Inner int can be incremented", "int") {
  locked<int> i(5);
  i.mut([](int& i) { i++; });
  REQUIRE(i.get() == 6);
}

TEST_CASE("Int incrementing race", "int") {
  const constexpr int count = 1000000;
  locked<int> l1(0);
  locked<int> l2(0);
  locked<int> l3(0);

  auto incrementer = [&l1, &l2, &l3, &count]() {
    static const auto increment = [](int& value) { value++; };
    for (int i = 0; i < count; i++) {
      l1.mut(increment);
      l2.mut(increment);
      l3.mut(increment);
    }
  };

  auto thread1 = std::thread(incrementer);
  auto thread2 = std::thread(incrementer);
  auto thread3 = std::thread(incrementer);

  thread1.join();
  thread2.join();
  thread3.join();

  REQUIRE(3 * count == l1.get());
  REQUIRE(3 * count == l2.get());
  REQUIRE(3 * count == l3.get());
}
