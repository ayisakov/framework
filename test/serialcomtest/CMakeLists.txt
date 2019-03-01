cmake_minimum_required(VERSION 2.8.9)
find_package( Boost COMPONENTS system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
set(serialcomtest_SOURCES
  ../../IOProvider.cpp
  ../../SerialPort.cpp
  ../../IOListener.cpp
  ../../UniqueReadBuffer.cpp
  ../../UniqueWriteBuffer.cpp
  ../../RelTimerMs.cpp
  serialcomtest.cpp)
add_executable(serialcomtest ${serialcomtest_SOURCES})
target_link_libraries(serialcomtest gtest gtest_main pthread ${Boost_LIBRARIES} )
