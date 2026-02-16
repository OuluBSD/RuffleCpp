# CMakeLists.txt for desktop component

# Check if wxWidgets is available
if(wxWidgets_FOUND)
    set(HAS_WXWIDGETS TRUE)
else()
    set(HAS_WXWIDGETS FALSE)
    message(WARNING "wxWidgets not found - desktop GUI will be limited")
endif()

# Create placeholder source files if no actual sources exist
set(DESKTOP_SOURCES "")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
    list(APPEND DESKTOP_SOURCES src/main.cpp)
else()
    # Create a minimal placeholder
    set(DESKTOP_SOURCES src/main_placeholder.cpp)
endif()

# Add the desktop executable
add_executable(ruffle_desktop
    ${DESKTOP_SOURCES}
    # Add other desktop source files as they are translated from Rust
)

# Set properties for the executable
set_target_properties(ruffle_desktop PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Link libraries
target_link_libraries(ruffle_desktop
    ruffle_core
    # Add other required libraries
)

if(wxWidgets_FOUND)
    target_link_libraries(ruffle_desktop ${wxWidgets_LIBRARIES})
endif()

# Include directories
target_include_directories(ruffle_desktop PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(wxWidgets_FOUND)
    target_include_directories(ruffle_desktop PUBLIC ${wxWidgets_INCLUDE_DIRS})
endif()

# Define compile definitions
target_compile_definitions(ruffle_desktop PRIVATE
    # Add compile definitions specific to desktop
)

# Handle Windows-specific resources (icon, version info)
if(WIN32)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets/favicon.ico")
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/assets/favicon.ico
            ${CMAKE_CURRENT_BINARY_DIR}/favicon.ico
            COPYONLY
        )

        # Create a .rc file for Windows resources
        set(WINDOWS_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/ruffle_desktop.rc)

        # Generate the resource file with icon
        file(WRITE ${WINDOWS_RC_FILE}
            "#define IDR_MAINFRAME 100\n"
            "IDR_MAINFRAME ICON \"${CMAKE_CURRENT_BINARY_DIR}/favicon.ico\"\n"
        )

        # Add the resource file to the target
        target_sources(ruffle_desktop PRIVATE ${WINDOWS_RC_FILE})
    endif()

    # Add version information to the executable
    set_target_properties(ruffle_desktop PROPERTIES
        VERSION 1.0.0
        SOVERSION 1
    )
endif()

# Add version information generation (equivalent to vergen functionality)
# This would typically involve generating a version header file
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
        @ONLY
    )

    # Add the generated version header to the include directories
    target_include_directories(ruffle_desktop PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Add a custom command to track changes to environment variables or git info
# This mimics the vergen functionality for version info
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE GIT_COMMIT_HASH
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)

    # Get commit timestamp
    execute_process(COMMAND ${GIT_EXECUTABLE} show -s --format=%ct HEAD
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE GIT_COMMIT_TIMESTAMP
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)

    # Get commit date
    execute_process(COMMAND ${GIT_EXECUTABLE} show -s --format=%ad --date=iso HEAD
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                    OUTPUT_VARIABLE GIT_COMMIT_DATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
endif()

# Create a custom target to regenerate version info when git changes
add_custom_target(version_info_check
    COMMAND ${CMAKE_COMMAND} -E echo "Checking version info..."
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Checking version information"
)