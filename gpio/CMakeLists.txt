add_library(gpio_driver INTERFACE)

target_include_directories(gpio_driver INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(gpio_driver INTERFACE
    cxx_std_20
)

# Platform-specific definitions
if (PLATFORM STREQUAL "host")
    target_compile_definitions(gpio_driver INTERFACE
        PLATFORM_HOST
    )
elseif (PLATFORM STREQUAL "stm32f1")
    target_compile_definitions(gpio_driver INTERFACE
        PLATFORM_STM32F1
    )

    add_library(cmsis_stm32f1 INTERFACE)

    target_include_directories(cmsis_stm32f1 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f1/Include
    )

    target_compile_definitions(cmsis_stm32f1 INTERFACE
        STM32F100xB
    )

    target_compile_options(gpio_driver INTERFACE
        -mcpu=cortex-m3
        -mthumb
        -ffunction-sections
        -fdata-sections
    )

    target_link_options(gpio_driver INTERFACE
        -Wl,--gc-sections
    )

    target_link_libraries(gpio_driver INTERFACE
        cmsis_stm32f1
    )

elseif (PLATFORM STREQUAL "stm32f3")
    target_compile_definitions(gpio_driver INTERFACE
        PLATFORM_STM32F3
    )

    add_library(cmsis_stm32f3 INTERFACE)

    target_include_directories(cmsis_stm32f3 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f3/Include
    )

    target_compile_definitions(cmsis_stm32f3 INTERFACE
        STM32F303xC
    )

    target_compile_options(gpio_driver INTERFACE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -ffunction-sections
        -fdata-sections
    )

    target_link_options(gpio_driver INTERFACE
        -Wl,--gc-sections
    )

    target_link_libraries(gpio_driver INTERFACE
        cmsis_stm32f3
    )

elseif (PLATFORM STREQUAL "stm32f7")
    target_compile_definitions(gpio_driver INTERFACE
        PLATFORM_STM32F7
    )

    add_library(cmsis_stm32f7 INTERFACE)

    target_include_directories(cmsis_stm32f7 INTERFACE
        ${CMAKE_SOURCE_DIR}/external/cmsis/Core/Include
        ${CMAKE_SOURCE_DIR}/external/cmsis-device-f7/Include
    )

    target_compile_definitions(cmsis_stm32f7 INTERFACE
        STM32F767xx
    )

    target_compile_options(gpio_driver INTERFACE
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -ffunction-sections
        -fdata-sections
    )

    target_link_options(gpio_driver INTERFACE
        -Wl,--gc-sections
    )

    target_link_libraries(gpio_driver INTERFACE
        cmsis_stm32f7
    )
endif()

if (BUILD_TESTS AND PLATFORM STREQUAL "host")
    if (BUILD_TESTS)
        target_compile_definitions(gpio_driver INTERFACE
            BUILD_TESTS
        )
    endif()
    add_subdirectory(tests)
endif()

