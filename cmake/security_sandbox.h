/*
 * C++ header for security sandbox functionality
 * This replaces the functionality of core/common/src/sandbox.rs
 */

#ifndef SECURITY_SANDBOX_H
#define SECURITY_SANDBOX_H

#include <string>
#include <utility>

// Forward declaration
struct HeaderExt;

// Type of sandbox that defines what a movie can access
// and how movies interact with each other.
//
// Note: sandbox type is defined *per SWF*.
enum class SandboxType {
    // The movie originates from a remote URL.
    //
    // In this case domain-based sandbox rules are used,
    // no filesystem access.
    REMOTE,

    // The movie is a local movie with filesystem access.
    //
    // This implies no network access.
    LOCAL_WITH_FILE,

    // The movie is a local movie with network access.
    //
    // This implies no filesystem access.
    LOCAL_WITH_NETWORK,

    // The movie is a trusted local movie with access to both filesystem and network.
    LOCAL_TRUSTED,

    // The movie is an AIR application with access to both filesystem and network.
    APPLICATION
};

// Class to encapsulate sandbox functionality
class SecuritySandbox {
public:
    // Infer sandbox type based on SWF URL and its header.
    //
    // When the URL is remote, SandboxType::REMOTE is used.
    // When the URL is local, SandboxType::LOCAL_WITH_FILE or
    // SandboxType::LOCAL_WITH_NETWORK is used depending on
    // the preference from the header.
    static SandboxType infer(const std::string& url, const HeaderExt& header);
    
    // Helper method to check if a sandbox type allows file access
    static bool allows_file_access(SandboxType type) {
        return type == SandboxType::LOCAL_WITH_FILE || 
               type == SandboxType::LOCAL_TRUSTED || 
               type == SandboxType::APPLICATION;
    }
    
    // Helper method to check if a sandbox type allows network access
    static bool allows_network_access(SandboxType type) {
        return type == SandboxType::REMOTE || 
               type == SandboxType::LOCAL_WITH_NETWORK || 
               type == SandboxType::LOCAL_TRUSTED || 
               type == SandboxType::APPLICATION;
    }
    
    // Helper method to check if a sandbox type is local
    static bool is_local(SandboxType type) {
        return type != SandboxType::REMOTE;
    }
    
    // Helper method to check if a sandbox type is trusted
    static bool is_trusted(SandboxType type) {
        return type == SandboxType::LOCAL_TRUSTED || type == SandboxType::APPLICATION;
    }
};

// Implementation of the infer method
inline SandboxType SecuritySandbox::infer(const std::string& url, const HeaderExt& header) {
    // Check if URL starts with "file://" to determine if it's local
    if (url.substr(0, 7) == "file://") {
        if (header.use_network_sandbox()) {
            return SandboxType::LOCAL_WITH_NETWORK;
        } else {
            return SandboxType::LOCAL_WITH_FILE;
        }
    } else {
        // Remote URL
        return SandboxType::REMOTE;
    }
}

// Structure representing header extension information
// This would need to be defined based on the actual SWF header structure
struct HeaderExt {
    // This would contain the actual fields from the SWF header
    // For now, we'll include a placeholder for the use_network_sandbox method
    bool network_sandbox_enabled = false;
    
    bool use_network_sandbox() const {
        return network_sandbox_enabled;
    }
};

#endif // SECURITY_SANDBOX_H