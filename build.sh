#!/bin/bash -ue

BUILD_SCRIPT_DIR=$(cd `dirname $0` && pwd)
BUILD_DIR=build

if [[ -e ${BUILD_DIR} ]] && [[ ! -e ${BUILD_DIR}/rules.ninja ]]; then
    echo "clean broken build folder and re-initialize it"
    rm -rf ${BUILD_DIR}
fi



if [[ ! -e ${BUILD_DIR} ]]; then
    # use subshell to configure the build
    (
        mkdir -p ${BUILD_DIR}
        cd ${BUILD_DIR}
        
        CMAKE_PARAMS=(
           # -D CMAKE_RULE_MESSAGES:BOOL=OFF
            -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON
        )
        

        #cmake  ${CMAKE_PARAMS[@]} -G Ninja ${BUILD_SCRIPT_DIR}  # for ninja
        cmake  ${CMAKE_PARAMS[@]} ${BUILD_SCRIPT_DIR}   # for make
    )
fi

cd ${BUILD_DIR}
#ninja
make