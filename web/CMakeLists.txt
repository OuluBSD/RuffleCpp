# CMakeLists.txt for web component

# Check if we're building for WASM
if(${CMAKE_SYSTEM_NAME} STREQUAL "Generic" AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "wasm")
    # Add the web/WASM library
    add_library(ruffle_web
        src/lib.rs.cpp  # This would be the translated lib.rs from web/
        # Add other web source files as they are translated from Rust
    )

    # Set properties for the library
    set_target_properties(ruffle_web PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    # Link libraries
    target_link_libraries(ruffle_web
        ruffle_core
        # Add other required libraries for web/WASM
    )

    # Include directories
    target_include_directories(ruffle_web PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    # Define compile definitions
    target_compile_definitions(ruffle_web PRIVATE
        # Add compile definitions specific to web
    )

    # For WASM target, increase stack size as in the original build.rs
    set_target_properties(ruffle_web PROPERTIES
        LINK_FLAGS "-Wl,-z,stack-size=2000000"
    )
else()
    # For non-WASM builds, we might still have web-related utilities
    add_library(ruffle_web_util
        src/util.cpp
        # Add other web utility source files
    )

    set_target_properties(ruffle_web_util PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(ruffle_web_util
        ruffle_core
    )

    target_include_directories(ruffle_web_util PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()

# Add emscripten-specific configurations if building for WASM with Emscripten
if(CMAKE_TOOLCHAIN_FILE MATCHES "emscripten")
    set_target_properties(ruffle_web PROPERTIES
        SUFFIX ".js"
    )
    
    # Additional Emscripten-specific link flags
    set_target_properties(ruffle_web PROPERTIES
        LINK_FLAGS "-s MODULARIZE=1 -s EXPORT_NAME='Ruffle'"
    )
endif()