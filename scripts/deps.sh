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
    popd
    popd
}

build-aws-sdk
