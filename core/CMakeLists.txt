# CMakeLists.txt for core component

# Create a placeholder source file if no actual sources exist
set(CORE_SOURCES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/lib.cpp")
    list(APPEND CORE_SOURCES src/lib.cpp)
endif()

# Add the core library
add_library(ruffle_core
    ${CORE_SOURCES}
    # Add other core source files as they are translated from Rust
)

# Set properties for the library
set_target_properties(ruffle_core PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Link libraries
target_link_libraries(ruffle_core
    # Add required libraries here
)

# Include directories
target_include_directories(ruffle_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Define compile definitions
target_compile_definitions(ruffle_core PRIVATE
    # Add compile definitions specific to core
)

# Handle playerglobal.swf generation (equivalent to build_playerglobal functionality)
# This would involve custom commands to build playerglobal.swf from AS files
# Since this is a build-time dependency, we'll add a custom command
# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/playerglobal.swf
#     COMMAND # Command to generate playerglobal.swf from ActionScript sources
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMENT "Building playerglobal.swf"
#     VERBATIM
# )

# Add the generated file as a dependency
# add_custom_target(playerglobal_swf ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/playerglobal.swf)

# Ensure the core library depends on playerglobal.swf
# add_dependencies(ruffle_core playerglobal_swf)

# Add a dependency tracking mechanism for avm2 globals
# This mimics the "rerun-if-changed=src/avm2/globals/" functionality
# set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/playerglobal.swf
#     PROPERTIES
#     OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/avm2/globals/"
# )