cmake_minimum_required(VERSION 2.8.9)
find_package( Boost COMPONENTS system REQUIRED )
find_package( GTest REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
set(framework_test_SOURCES
  TestApp.cpp
  ../ThreadRunner.cpp
  ../SimpleLogger.cpp
  TestMessages.cpp
  ../IOProvider.cpp
  ../SerialPort.cpp
  ../IOListener.cpp
  ../RelTimerMs.cpp
  ../TestArgument.cpp
  test.cpp)
add_subdirectory(serialcomtest)
add_executable(framework_test ${framework_test_SOURCES})
target_link_libraries(framework_test GTest::GTest GTest::Main pthread ${Boost_LIBRARIES} )
