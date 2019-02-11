cmake_minimum_required(VERSION 2.8.9)
set(framework_test_SOURCES
  TestApp.cpp
  TestMessages.cpp
  test.cpp)
add_executable(framework_test ${framework_test_SOURCES})
target_link_libraries(framework_test gtest gtest_main)
