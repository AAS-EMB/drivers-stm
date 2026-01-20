
add_executable(gpio_driver_tests
    gpio_test.cpp
   # compile_check.cpp
)

target_link_libraries(gpio_driver_tests
    PRIVATE
        gpio_driver
        gtest
        gmock
        gtest_main
)

add_test(
    NAME gpio_driver_tests
    COMMAND gpio_driver_tests
)
