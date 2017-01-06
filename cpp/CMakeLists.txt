cmake_minimum_required(VERSION 2.8)
project(gcolor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES src/main.cpp src/graph.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_executable(gcolor ${SOURCE_FILES})

# Google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(codeToTest ${SOURCE_FILES})
add_executable(runTests test/test.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread codeToTest)