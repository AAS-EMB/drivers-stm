cmake_minimum_required(VERSION 3.20)
project(drivers_stm LANGUAGES C CXX)

option(BUILD_TESTS "Build unit tests" ON)
option(PLATFORM "Target platform" "host")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (BUILD_TESTS)
    enable_testing()
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/googletest)
endif()

add_subdirectory(chrono_clocks)
