
#define CATCH_CONFIG_MAIN

#include "Catch2/single_include/catch2/catch.hpp"
#include "locked.hpp"

TEST_CASE("Inner int can be incremented", "int") {
  locked<int> i(5);
  i.mut([](int& i) { i++; });
  REQUIRE(i.get() == 6);
}

