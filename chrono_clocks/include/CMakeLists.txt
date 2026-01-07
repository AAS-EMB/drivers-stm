add_library(chrono_clocks INTERFACE)

target_include_directories(chrono_clocks
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(chrono_clocks
    INTERFACE cxx_std_20
)

target_compile_definitions(chrono_clocks
    INTERFACE
        CHRONO_HEADER_ONLY
)
