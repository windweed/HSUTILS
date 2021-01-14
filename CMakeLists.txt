cmake_minimum_required(VERSION 2.8)

project(HyperScan_Util)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILE_FEATURES -Wall -g -ggdb)
set(CMAKE_CXX_COMPILER g++)

# 获取epoch编译时间
execute_process(
    COMMAND date "+%s"                # "+%Y-%m-%d %H:%M:%S"
    OUTPUT_VARIABLE BUILD_TIME_EPOCH
)
string(REGEX REPLACE "\r?\n" "" BUILD_TIME ${BUILD_TIME_EPOCH})

# 有5个宏需要CMake定义：
set(BUILD_TIME ${BUILD_TIME}) # 编译时间
set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_SUB   3)
set(TXT_CFG_DIR \"${CMAKE_SOURCE_DIR}/demo/\")       # 配置文件位置
set(DB_OUTPUT_DIR \"${CMAKE_SOURCE_DIR}/db/\")      # 输出db的位置
set(DB_CFG_DIR \"${CMAKE_SOURCE_DIR}/db/\")

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.h.in
    ${PROJECT_BINARY_DIR}/build_config.h
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/3rdpart/hyperscan
    # demo
    ${CMAKE_SOURCE_DIR}/demo
    # build_config.h
    ${CMAKE_BINARY_DIR}/
)

aux_source_directory(${CMAKE_SOURCE_DIR}/demo demofiles)
aux_source_directory(${CMAKE_SOURCE_DIR}/src srcfiles)

add_executable(Demo ${demofiles} ${srcfiles})

target_link_directories(Demo
    PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdpart/hyperscan
)

target_link_libraries(Demo
    PUBLIC
        libhs.so
)
