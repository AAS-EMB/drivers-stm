
add_executable(chrono_clocks_tests
    steady_timer_test.cpp
    system_timer_test.cpp
)

target_link_libraries(chrono_clocks_tests
    PRIVATE
        chrono_clocks
        gtest
        gmock
        gtest_main
)

add_test(
    NAME chrono_clocks_tests
    COMMAND chrono_clocks_tests
)
