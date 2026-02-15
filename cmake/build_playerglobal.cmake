# CMake script to build playerglobal.swf equivalent for RuffleCpp
# This replaces the Rust build.rs functionality

include_guard()

# Function to build playerglobal.swf from ActionScript sources
function(build_playerglobal_cpp OUT_DIR_VAR REPO_ROOT)
    # Define the output directory for generated files
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/playerglobal_gen)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})

    # Path to the ActionScript source files
    set(CLASSES_DIR ${REPO_ROOT}/core/src/avm2/globals/)
    
    # Path to the asc.jar compiler (would need to be provided separately)
    set(ASC_JAR_PATH ${REPO_ROOT}/core/build_playerglobal/asc.jar)
    
    # Check if Java is available
    find_package(Java COMPONENTS Runtime REQUIRED)
    if(NOT JAVA_RUNTIME_FOUND)
        message(FATAL_ERROR "Java is required to build playerglobal.swf but was not found.")
    endif()

    # Define the output SWF file
    set(PLAYERGLOBAL_SWF ${OUTPUT_DIR}/playerglobal.swf)
    
    # Define ActionScript source files to compile
    set(AS_SOURCES 
        ${CLASSES_DIR}/Toplevel.as
        ${CLASSES_DIR}/globals.as
    )
    
    # Add custom command to compile ActionScript to ABC using asc.jar
    add_custom_command(
        OUTPUT ${OUTPUT_DIR}/playerglobal.abc
        COMMAND ${JAVA_RUNTIME} -classpath ${ASC_JAR_PATH}
            macromedia.asc.embedding.ScriptCompiler
            -optimize
            -builtin
            -apiversioning
            -version 9
            -outdir ${OUTPUT_DIR}
            -out playerglobal
            ${AS_SOURCES}
        DEPENDS ${AS_SOURCES} ${ASC_JAR_PATH}
        COMMENT "Compiling ActionScript to ABC..."
        VERBATIM
    )
    
    # Custom command to process the ABC file and generate the final SWF
    # This would require a custom C++ tool to replicate the Rust functionality
    add_custom_command(
        OUTPUT ${PLAYERGLOBAL_SWF}
        COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_DIR}/playerglobal.abc ${PLAYERGLOBAL_SWF}
        # In a real implementation, we would have a C++ tool that replicates the Rust build_playerglobal functionality
        # This is a placeholder - a real implementation would need a C++ equivalent of the Rust build_playerglobal lib
        DEPENDS ${OUTPUT_DIR}/playerglobal.abc
        COMMENT "Processing ABC to SWF..."
        VERBATIM
    )
    
    # Set the output variable with the path to the generated SWF
    set(${OUT_DIR_VAR} ${OUTPUT_DIR} PARENT_SCOPE)
    
    # Add a custom target for building playerglobal
    add_custom_target(playerglobal_swf ALL
        DEPENDS ${PLAYERGLOBAL_SWF}
        COMMENT "Building playerglobal.swf"
    )
    
    # Mark the SWF file as a generated resource
    set_source_files_properties(${PLAYERGLOBAL_SWF} PROPERTIES GENERATED TRUE)
endfunction()

# Alternative implementation using a C++ tool that replicates the Rust build_playerglobal functionality
function(build_playerglobal_cpp_tool OUT_DIR_VAR REPO_ROOT WITH_STUBS)
    # Define the output directory for generated files
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/playerglobal_gen)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    
    # Path to the C++ tool that replicates build_playerglobal functionality
    # This tool would need to be implemented as part of the C++ translation
    set(BUILD_PLAYERGLOBAL_TOOL $<TARGET_FILE:build_playerglobal_tool>)
    
    # Define the output SWF file
    set(PLAYERGLOBAL_SWF ${OUTPUT_DIR}/playerglobal.swf)
    
    # Add custom command to run the C++ build_playerglobal tool
    if(WITH_STUBS)
        add_custom_command(
            OUTPUT ${PLAYERGLOBAL_SWF}
            COMMAND ${BUILD_PLAYERGLOBAL_TOOL} ${REPO_ROOT} ${OUTPUT_DIR} --with-stubs
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS build_playerglobal_tool  # The C++ tool that replicates Rust functionality
            COMMENT "Generating playerglobal.swf with C++ tool..."
            VERBATIM
        )
    else()
        add_custom_command(
            OUTPUT ${PLAYERGLOBAL_SWF}
            COMMAND ${BUILD_PLAYERGLOBAL_TOOL} ${REPO_ROOT} ${OUTPUT_DIR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            DEPENDS build_playerglobal_tool  # The C++ tool that replicates Rust functionality
            COMMENT "Generating playerglobal.swf with C++ tool..."
            VERBATIM
        )
    endif()
    
    # Set the output variable with the path to the generated SWF
    set(${OUT_DIR_VAR} ${OUTPUT_DIR} PARENT_SCOPE)
    
    # Add a custom target for building playerglobal
    add_custom_target(playerglobal_swf ALL
        DEPENDS ${PLAYERGLOBAL_SWF}
        COMMENT "Building playerglobal.swf with C++ tool"
    )
    
    # Mark the SWF file as a generated resource
    set_source_files_properties(${PLAYERGLOBAL_SWF} PROPERTIES GENERATED TRUE)
endfunction()

# Example usage:
# build_playerglobal_cpp(PLAYERGLOBAL_OUTPUT_DIR ${CMAKE_SOURCE_DIR})