/*
 * C++ equivalent of the Rust CLI module for build_playerglobal
 * This replaces the functionality of core/build_playerglobal/src/cli.rs
 */

#ifndef BUILD_PLAYERGLOBAL_CLI_H
#define BUILD_PLAYERGLOBAL_CLI_H

#include <string>
#include <variant>
#include <iostream>
#include <optional>

// Enum to represent the different commands
enum class CommandType {
    COMPILE
};

// Struct to hold compile command parameters
struct CompileCommand {
    std::string out_dir;
};

// Variant to hold different command types
using Command = std::variant<CompileCommand>;

// Main CLI structure
struct Cli {
    Command command;
    
    // Parse command line arguments
    static std::optional<Cli> parse(int argc, char* argv[]) {
        if (argc < 2) {
            print_usage();
            return std::nullopt;
        }
        
        std::string cmd = argv[1];
        
        if (cmd == "compile") {
            if (argc < 3) {
                std::cerr << "Error: compile command requires an output directory" << std::endl;
                print_usage();
                return std::nullopt;
            }
            
            CompileCommand compile_cmd;
            compile_cmd.out_dir = argv[2];
            
            return Cli{compile_cmd};
        } else {
            std::cerr << "Error: Unknown command '" << cmd << "'" << std::endl;
            print_usage();
            return std::nullopt;
        }
    }
    
private:
    static void print_usage() {
        std::cout << "Usage: build_playerglobal <command> [options]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  compile <out_dir>    Compile playerglobal with output to <out_dir>" << std::endl;
    }
};

#endif // BUILD_PLAYERGLOBAL_CLI_H