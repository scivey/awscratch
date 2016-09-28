cmake_minimum_required(VERSION 2.8)

project(awscratch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall -O0 -g")

set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(GTEST ${EXTERNAL}/googletest)

set(AWS_ROOT ${EXTERNAL}/aws-sdk-cpp)
set(AWS_BUILD ${AWS_ROOT}/build)
set(S3_DIR ${AWS_ROOT}/aws-cpp-sdk-s3)

set(AWS_INCLUDES
    ${S3_DIR}/include
    ${AWS_ROOT}/aws-cpp-sdk-core/include
    ${AWS_ROOT}/aws-cpp-sdk-elasticache/include
    ${AWS_ROOT}/aws-cpp-sdk-ec2/include

)

set(AWS_LINKS
    ${AWS_BUILD}/aws-cpp-sdk-s3
    ${AWS_BUILD}/aws-cpp-sdk-core
    ${AWS_BUILD}/aws-cpp-sdk-elasticache
    ${AWS_BUILD}/aws-cpp-sdk-ec2
)

include_directories(
    ${GTEST}/googletest/include
    ${GTEST}/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${AWS_INCLUDES}
    /usr/local/include
)

link_directories(
    ${GTEST}/build/googlemock
    ${AWS_LINKS}
    /usr/local/lib
)

set(COMMON_LIBS
    folly
    event
    glog
    aws-cpp-sdk-s3
    aws-cpp-sdk-core
    aws-cpp-sdk-elasticache
    aws-cpp-sdk-ec2
    crypto
    curl
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
