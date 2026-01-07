add_library(chrono_clocks INTERFACE)

target_include_directories(chrono_clocks
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(chrono_clocks
    INTERFACE cxx_std_20
)

# Platform-specific definitions
if (PLATFORM STREQUAL "host")
    target_compile_definitions(chrono_clocks
        INTERFACE CHRONO_PLATFORM_HOST
    )
elseif (PLATFORM STREQUAL "stm32f7")
    target_compile_definitions(chrono_clocks
        INTERFACE CHRONO_PLATFORM_STM32F7
    )
endif()

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
