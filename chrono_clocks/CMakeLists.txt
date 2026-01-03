cmake_minimum_required(VERSION 3.20)

project(chrono_clocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(CHRONO_BUILD_TESTS "Build unit tests" ON)

# =========================
# Header-only INTERFACE lib
# =========================
add_library(chrono_clocks INTERFACE)

target_include_directories(chrono_clocks
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(chrono_clocks
    INTERFACE cxx_std_20
)

# =========================
# Platform selection
# =========================
if (NOT CHRONO_PLATFORM)
    message(FATAL_ERROR "CHRONO_PLATFORM not set (host / stm32f7)")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/platforms/${CHRONO_PLATFORM}.cmake)

# =========================
# Tests (host only)
# =========================
if (CHRONO_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
