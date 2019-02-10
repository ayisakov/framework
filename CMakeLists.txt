cmake_minimum_required(VERSION 2.8.9)
project(framework)
# enable GTest
enable_testing()
add_subdirectory(test)
add_test(framework_test test/framework_test)
