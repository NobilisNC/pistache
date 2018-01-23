#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define PISTACHE_IMPL 1
#include "./pistache.h"

TEST_CASE("String formatter", "") {
  pistache::String str("Hello {name}!");
  pistache::Context ctx = {{"name" , "world"}};

  REQUIRE(str.get(ctx) == "Hello world!");

  REQUIRE(str.fragments().size() == 2);
  REQUIRE(str.placeholders().size() == 1);

  REQUIRE(str({{"name", "foo"}}) == "Hello foo!");

  str.setTemplate("{first} {second} {first}");

  REQUIRE(str({{"first", "a"}, {"second", "b"}}) == "a b a");
}
