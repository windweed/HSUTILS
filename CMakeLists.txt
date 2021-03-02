cmake_minimum_required(VERSION 3.8)

project(HyperScan_Util)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_COMPILER g++)
# set(CMAKE_CXX_COMPILE_FEATURES -Wall -g -ggdb)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# get compile time (epoch)
execute_process(
    COMMAND date "+%s"     # "+%Y-%m-%d %H:%M:%S" to 2021-01-01 10:00:00
    OUTPUT_VARIABLE BUILD_TIME_EPOCH
)
string(REGEX REPLACE "\r?\n" "" BUILD_TIME ${BUILD_TIME_EPOCH})

#### MODIFY BELOW
set(DB_OUTPUT_DIR \"${CMAKE_SOURCE_DIR}/db/output/\")      # path of output db
set(DB_INPUT_DIR \"${CMAKE_SOURCE_DIR}/db/input/\")
set(TXT_CFG_DIR \"${CMAKE_SOURCE_DIR}/app/\")
set(VER_MAJOR 1)
set(VER_MINOR 1)
set(VER_PATCH 1)
####

# DEMO
set(DEMO_VER_MAJOR 6)
set(DEMO_VER_MINOR 7)
set(DEMO_VER_PATCH 8)
set(DEMO_TXT_CFG_DIR \"${CMAKE_SOURCE_DIR}/demo/\")       # path of cfg file

configure_file(
    ${PROJECT_SOURCE_DIR}/src/config.h.in
    ${PROJECT_BINARY_DIR}/build_config.h
    @ONLY
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

add_library(czyhsutils SHARED ${srcfiles})

# package rpm
set(CPACK_GENERATOR RPM)
set(CPACK_RPM_PACKAGE_AUTOREQPROV no)
set(CPACK_PACKAGING_INSTALL_PREFIX /home/czy/install) # default /usr/local
install(PROGRAMS build/bin/Demo DESTINATION bin)
install(FILES src/serializer.h DESTINATION include)
install(FILES src/deserializer.h DESTINATION include)
install(FILES ${CMAKE_BINARY_DIR}/lib/libczyhsutils.so DESTINATION lib)
set(CPACK_RPM_PACKAGE_NAME "czy-hsutils")
set(CPACK_RPM_PACKAGE_SUMMARY "Simple CPack HSUTILS")
set(CPACK_RPM_PACKAGE_VENDOR "CZYWINDWEED")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "3")
set(CPACK_RPM_PACKAGE_GROUP "windweed")
set(CPACK_RPM_PACKAGE_URL "github.com/windweed")
set(CPACK_RPM_PACKAGE_DESCRIPTION "windweed, hstest")
set(CPACK_RPM_PACKAGE_RELEASE 1)
set(CPACK_RPM_PACKAGE_LICENSE "MIT Licence")
include(CPack)
