add_library(gpio_driver INTERFACE)

target_include_directories(gpio_driver
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(gpio_driver
    INTERFACE cxx_std_20
)

target_compile_definitions(gpio_driver
    INTERFACE
        GPIO_HEADER_ONLY
)
