cmake_minimum_required(VERSION 3.20)
project(drivers_stm LANGUAGES C CXX)

option(BUILD_TESTS "Build unit tests" ON)
option(PLATFORM "Target platform" "host")

# Drivers options
option(CHRONO_CLOCKS_LIB "Chrono clocks library" OFF)
option(GPIO_LIB "Gpio library" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CHRONO_CLOCKS_LIB ON)
    set(GPIO_LIB ON)
endif()

if (BUILD_TESTS)
    add_compile_definitions(BUILD_TESTS)
endif()

# Platform-specific definitions
if (PLATFORM STREQUAL "host")
    add_compile_definitions(PLATFORM_HOST)
elseif (PLATFORM STREQUAL "stm32f1")
    add_compile_definitions(PLATFORM_STM32F1)

    add_library(cmsis_stm32f1 INTERFACE)

    target_include_directories(cmsis_stm32f1 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f1/Include
    )

    set(COMMON_COMPILE_OPTIONS
        -mcpu=cortex-m3
    )

    set(COMMON_LINK_OPTIONS
        -mcpu=cortex-m3
        -mthumb
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        -Wl,--start-group
        -Wl,--end-group
        -specs=nano.specs
        -specs=nosys.specs
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m3 -mthumb")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_LINK_OPTIONS}")

    if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        target_compile_definitions(cmsis_stm32f1 INTERFACE
            STM32F100xB
        )
    endif()
elseif (PLATFORM STREQUAL "stm32f3")
    add_compile_definitions(PLATFORM_STM32F3)

    add_library(cmsis_stm32f3 INTERFACE)

    target_include_directories(cmsis_stm32f3 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f3/Include
    )

    set(COMMON_COMPILE_OPTIONS
        -mcpu=cortex-m4
    )

    set(COMMON_LINK_OPTIONS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        -Wl,--start-group
        -Wl,--end-group
        -specs=nano.specs
        -specs=nosys.specs
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m4 -mthumb")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_LINK_OPTIONS}")

    if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        target_compile_definitions(cmsis_stm32f3 INTERFACE
            STM32F303xC
        )
    endif()
elseif (PLATFORM STREQUAL "stm32f7")
    add_compile_definitions(PLATFORM_STM32F7)

    add_library(cmsis_stm32f7 INTERFACE)

    target_include_directories(cmsis_stm32f7 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f7/Include
    )

    set(COMMON_COMPILE_OPTIONS
        -mcpu=cortex-m7
    )

    set(COMMON_LINK_OPTIONS
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -Wl,--gc-sections
        -Wl,--print-memory-usage
        -Wl,--start-group
        -Wl,--end-group
        -specs=nano.specs
        -specs=nosys.specs
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_COMPILE_OPTIONS}")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mcpu=cortex-m7 -mthumb")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${COMMON_LINK_OPTIONS}")

    if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
        target_compile_definitions(cmsis_stm32f7 INTERFACE
            STM32F767xx
        )
    endif()
endif()

if (BUILD_TESTS)
    enable_testing()
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
    add_subdirectory(external/googletest)
endif()

if (CHRONO_CLOCKS_LIB)
    add_subdirectory(chrono_clocks)
endif()
if (GPIO_LIB)
    add_subdirectory(gpio)
endif()
