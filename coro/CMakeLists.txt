add_library(async_lib INTERFACE)

target_include_directories(async_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(async_lib INTERFACE
    cxx_std_20
)

# Platform-specific definitions
if (PLATFORM STREQUAL "stm32f1")
    target_link_libraries(async_lib INTERFACE
        cmsis_stm32f1
    )
elseif (PLATFORM STREQUAL "stm32f3")
    target_link_libraries(async_lib INTERFACE
        cmsis_stm32f3
    )
elseif (PLATFORM STREQUAL "stm32f7")
    target_link_libraries(async_lib INTERFACE
        cmsis_stm32f7
    )
endif()

if (BUILD_TESTS AND PLATFORM STREQUAL "host")
    add_subdirectory(tests)
endif()

if (NOT PLATFORM STREQUAL "host" AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_library(chrono_compile_check OBJECT
        ${CMAKE_CURRENT_LIST_DIR}/tests/compile_check.cpp
    )

    target_link_libraries(chrono_compile_check PRIVATE
        async_lib
    )
endif()
