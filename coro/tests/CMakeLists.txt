
add_executable(async_lib_tests
    async_flag_test.cpp
)

target_link_libraries(async_lib_tests
    PRIVATE
        async_lib
        gtest
        gmock
        gtest_main
)

add_test(
    NAME async_lib_tests
    COMMAND async_lib_tests
)
