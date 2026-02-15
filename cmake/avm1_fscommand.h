/*
 * C++ header for AVM1 FSCommand functionality
 * This replaces the functionality of core/src/avm1/fscommand.rs
 */

#ifndef AVM1_FSCOMMAND_H
#define AVM1_FSCOMMAND_H

#include "avm1.h"
#include "avm1/activation.h"
#include "avm1/error.h"
#include <string>
#include <memory>
#include <optional>

namespace ruffle {

// FSCommand handling functionality
class FsCommand {
public:
    // Parse an FSCommand URL
    static std::optional<std::string> parse(const std::string& url) {
        const std::string prefix = "fscommand:";
        
        if (url.length() < prefix.length()) {
            return std::nullopt;
        }

        std::string head = url.substr(0, prefix.length());
        std::string tail = url.substr(prefix.length());

        // Case-insensitive comparison
        if (equalsIgnoreCase(head, prefix)) {
            return tail;
        } else {
            return std::nullopt;
        }
    }

    // Handle an FSCommand
    static bool handle(const std::string& command,
                      const std::string& args,
                      std::shared_ptr<Activation> activation) {
        try {
            // Try to invoke the command through external interface
            bool handled = activation->context()->external_interface.invoke_fs_command(command, args);
            
            if (!handled) {
                // Log warning for unhandled command
                AVM_WARN(activation, "Unhandled FSCommand: %s", command.c_str());
            }
            
            return true;  // Operation completed (even if not handled)
        } catch (const std::exception& e) {
            // Handle any errors that occurred during command processing
            Avm1Error error = Avm1Error::execution_timeout(); // Use appropriate error
            throw Avm1Exception(error);
        }
    }

private:
    // Helper function for case-insensitive string comparison
    static bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
        if (str1.length() != str2.length()) {
            return false;
        }
        
        for (size_t i = 0; i < str1.length(); ++i) {
            if (tolower(str1[i]) != tolower(str2[i])) {
                return false;
            }
        }
        return true;
    }
};

// External interface class that would handle FSCommands
class ExternalInterface {
public:
    // Invoke an FSCommand
    bool invoke_fs_command(const std::string& command, const std::string& args) {
        // In a real implementation, this would communicate with the host environment
        // For now, we'll implement some basic commands
        
        if (command == "quit" || command == "Quit") {
            // Handle quit command
            return handle_quit_command(args);
        } else if (command == "allowscale" || command == "AllowScale") {
            // Handle allow scale command
            return handle_allow_scale_command(args);
        } else if (command == "trapallkeys" || command == "TrapAllKeys") {
            // Handle trap all keys command
            return handle_trap_all_keys_command(args);
        } else if (command == "exec" || command == "Exec") {
            // Handle exec command (potentially dangerous, usually disabled)
            return handle_exec_command(args);
        } else if (command == "enableDebugger" || command == "EnableDebugger") {
            // Handle debugger enable command
            return handle_enable_debugger_command(args);
        }
        
        // Command not handled by this interface
        return false;
    }

private:
    bool handle_quit_command(const std::string& args) {
        // In a real implementation, this would terminate the player
        // For safety, this is often disabled
        return false;
    }
    
    bool handle_allow_scale_command(const std::string& args) {
        // In a real implementation, this would control scaling behavior
        // Args would be "true" or "false"
        return true;
    }
    
    bool handle_trap_all_keys_command(const std::string& args) {
        // In a real implementation, this would control keyboard event handling
        // Args would be "true" or "false"
        return true;
    }
    
    bool handle_exec_command(const std::string& args) {
        // This command is typically disabled for security reasons
        return false;
    }
    
    bool handle_enable_debugger_command(const std::string& args) {
        // In a real implementation, this would control debugger features
        // Args would be "true" or "false"
        return true;
    }
};

} // namespace ruffle

#endif // AVM1_FSCOMMAND_H