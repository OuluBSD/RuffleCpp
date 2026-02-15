/*
 * C++ main function for build_playerglobal utility
 * This replaces the functionality of core/build_playerglobal/src/main.rs
 */

#include "build_playerglobal_lib.h"
#include "build_playerglobal_cli.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    try {
        // Determine the repository root (two levels up from current executable location)
        // In a real implementation, this would be determined differently
        fs::path repo_root = "../../";  // Default fallback
        
        // Parse command line arguments
        auto cli = Cli::parse(argc, argv);
        if (!cli.has_value()) {
            return 1; // Error in parsing
        }
        
        const auto& command = cli->command;
        
        // Process the command
        if (std::holds_alternative<CompileCommand>(command)) {
            const auto& compile_cmd = std::get<CompileCommand>(command);
            
            // Call the build function
            if (!build_playerglobal(repo_root, compile_cmd.out_dir, false)) {
                std::cerr << "Error: Failed to build playerglobal" << std::endl;
                return 1;
            }
            
            std::cout << "Successfully built playerglobal.swf to " << compile_cmd.out_dir << std::endl;
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}