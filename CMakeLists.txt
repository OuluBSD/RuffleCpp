cmake_minimum_required(VERSION 3.20)

# Set project name and language
project(RuffleCpp LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Enable useful warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Add platform-specific configurations
if(WIN32)
    # Increase stack size for Windows platforms to accommodate SWF recursion
    # Similar to the /STACK:4000000 flag in the original build.rs
    if(MSVC)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:4000000")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,4000000")
    endif()
endif()

# For WASM target, increase stack size
# This corresponds to the web/build.rs functionality
if(${CMAKE_SYSTEM_NAME} STREQUAL "Generic" AND ${CMAKE_SYSTEM_PROCESSOR} MATCHES "wasm")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,stack-size=2000000")
endif()

# Find required packages - wxWidgets is optional for now
find_package(wxWidgets COMPONENTS core base)

# Add subdirectories for different components
add_subdirectory(core)
add_subdirectory(desktop)
add_subdirectory(web)

# Define common include directories
include_directories(${PROJECT_SOURCE_DIR}/core/include)
include_directories(${PROJECT_SOURCE_DIR}/common/include)

# Define common compile definitions
add_compile_definitions(PROJECT_ROOT="${PROJECT_SOURCE_DIR}")

# Set release channel (similar to CFG_RELEASE_CHANNEL in Rust)
if(DEFINED ENV{CFG_RELEASE_CHANNEL})
    add_compile_definitions(CFG_RELEASE_CHANNEL="$ENV{CFG_RELEASE_CHANNEL}")
else()
    add_compile_definitions(CFG_RELEASE_CHANNEL="local")
endif()

# Print configuration summary
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "wxWidgets found: ${wxWidgets_FOUND}")
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    message(STATUS "wxWidgets libraries: ${wxWidgets_LIBRARIES}")
endif()