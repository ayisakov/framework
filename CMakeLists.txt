cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
project(framework)
# enable GTest
enable_testing()
add_subdirectory(googletest)
add_subdirectory(test)
add_test(framework_test test/framework_test)
