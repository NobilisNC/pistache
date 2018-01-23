cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(pistache VERSION 0.1.0 LANGUAGES CXX)

option(BUILD_TESTS "Build Tests" ON)

include(pistache.cmake)

if( BUILD_TESTS )
  include(deps/catch2.cmake)
  enable_testing()
  add_subdirectory(tests)
endif()
