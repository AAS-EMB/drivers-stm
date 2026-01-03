include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(chrono_clocks_tests
    steady_timer_test.cpp
)

target_link_libraries(chrono_clocks_tests
    PRIVATE
        chrono_clocks
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(chrono_clocks_tests)
