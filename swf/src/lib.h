// C++ translation of swf/src/lib.rs
//! Library for reading and writing Adobe Flash SWF files.

#ifndef RUFFLE_SWF_LIB_H
#define RUFFLE_SWF_LIB_H

// SWF submodules
namespace ruffle {
namespace swf {

// AVM1 module
namespace avm1 {
    // AVM1 reading and writing functionality
}

// AVM2 module
namespace avm2 {
    // AVM2 reading and writing functionality
}

// Error types
namespace error {
    class Error;
}

// Extensions module
namespace extensions {
    // SWF extensions
}

// Reading functionality
namespace read {
    /// Decompress a SWF file
    /// @param data The compressed SWF data
    /// @return The decompressed SWF data
    std::vector<uint8_t> decompress_swf(const std::vector<uint8_t>& data);

    /// Parse a SWF file
    /// @param data The SWF data
    /// @return The parsed SWF movie
    class SwfMovie;
    SwfMovie parse_swf(const std::vector<uint8_t>& data);
}

// String utilities
namespace string {
    /// SWF string type
    class SwfStr;
}

// Tag codes
namespace tag_code {
    /// SWF tag code enumeration
    enum class TagCode : uint16_t;
}

// Types module
namespace types {
    // SWF type definitions
}

// Writing functionality
namespace write {
    /// Write a SWF file
    /// @param movie The SWF movie to write
    /// @param compression The compression method to use
    /// @return The serialized SWF data
    std::vector<uint8_t> write_swf(const read::SwfMovie& movie, uint8_t compression);
}

// Re-exports
using read::decompress_swf;
using read::parse_swf;
using string::SwfStr;
using tag_code::TagCode;
// Types re-exports would be defined here
using write::write_swf;

} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_LIB_H
