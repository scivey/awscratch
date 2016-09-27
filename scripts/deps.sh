#!/bin/bash

ROOT=$(git rev-parse --show-toplevel)

EXT=${ROOT}/external

function cmake-build() {
    if [[ ! -d "build" ]]; then
        mkdir build
        pushd build
        cmake ../ && make -j8
        popd
    fi
}

pushd ${EXT}/googletest
cmake-build
popd


function build-aws-sdk() {
    pushd ${EXT}/aws-sdk-cpp
    if [[ ! -d "build" ]]; then
        mkdir build
        pushd build
        cmake -DBUILD_SHARED_LIBS=OFF ../
        popd
    fi
    pushd build

    local libs="s3 ec2 elasticache dynamodb rds core"
    for lib in $libs; do
        if [[ ! -f "aws-cpp-sdk-${lib}/libaws-cpp-sdk-${lib}.a" ]]; then
            make aws-cpp-sdk-${lib} -j8
        fi
    done
    # if [[ ! -f "aws-cpp-sdk-s3/libaws-cpp-sdk-s3.a" ]]; then
    #     make aws-cpp-sdk-s3 -j8
    # fi

    # if [[ ! -f "aws-cpp-sdk-ec2/libaws-cpp-sdk-ec2.a" ]]; then
    #     make aws-cpp-sdk-ec2 -j8
    # fi

    # if [[ ! -f "aws-cpp-sdk-elasticache/libaws-cpp-sdk-elasticache.a" ]]; then
    #     make aws-cpp-sdk-elasticache -j8
    # fi

    popd
    popd
}

build-aws-sdk