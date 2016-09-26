cmake_minimum_required(VERSION 2.8)

project(awscratch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -O0 -g")

set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(GTEST ${EXTERNAL}/googletest)

include_directories(
    ${GTEST}/googletest/include
    ${GTEST}/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/local/include
)

link_directories(
    ${GTEST}/build/googlemock
    /usr/local/lib
)

set(COMMON_LIBS
    folly
    event
    glog
    pthread
    atomic
)

set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

FILE(GLOB AWSCRATCH_CORE_SRC
    ${SRC_ROOT}/awscratch/*.cpp
    ${SRC_ROOT}/awscratch/**/*.cpp
)

add_library(awscratch
    ${AWSCRATCH_CORE_SRC}
)
target_link_libraries(awscratch ${COMMON_LIBS})

add_executable(runner
    ${SRC_ROOT}/main.cpp
)

add_dependencies(runner awscratch)
target_link_libraries(runner awscratch ${COMMON_LIBS})


FILE(GLOB AWSCRATCH_TEST_SRC
    ${SRC_ROOT}/test/*.cpp
    ${SRC_ROOT}/test/**/*.cpp
)
add_executable(test_runner
    ${AWSCRATCH_TEST_SRC}
    ${SRC_ROOT}/run_tests.cpp
)
add_dependencies(test_runner awscratch)

target_link_libraries(test_runner
    awscratch
    gmock
    ${COMMON_LIBS}
)
