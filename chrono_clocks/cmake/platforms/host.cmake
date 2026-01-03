message(STATUS "Configuring chrono_clocks for HOST")

target_compile_definitions(chrono_clocks
    INTERFACE
        CHRONO_PLATFORM_HOST
)
