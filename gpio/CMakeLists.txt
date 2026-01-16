add_library(gpio_driver INTERFACE)

target_include_directories(gpio_driver INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(gpio_driver INTERFACE
    cxx_std_20
)

# Platform-specific definitions
if (PLATFORM STREQUAL "stm32f1")
    target_link_libraries(chrono_clocks INTERFACE
        cmsis_stm32f1
    )
elseif (PLATFORM STREQUAL "stm32f3")
    target_link_libraries(chrono_clocks INTERFACE
        cmsis_stm32f3
    )
elseif (PLATFORM STREQUAL "stm32f7")
    target_link_libraries(chrono_clocks INTERFACE
        cmsis_stm32f7
    )
endif()

if (BUILD_TESTS AND PLATFORM STREQUAL "host")
    add_subdirectory(tests)
endif()

