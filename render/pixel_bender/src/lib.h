// C++ translation of render/pixel_bender/src/lib.rs
// Pixel Bender module declarations

#ifndef RUFFLE_RENDER_PIXEL_BENDER_LIB_H
#define RUFFLE_RENDER_PIXEL_BENDER_LIB_H

namespace ruffle {
namespace render {
namespace pixel_bender {

// Test module (conditionally compiled)
namespace tests {
    // Test functionality
}

// Assembly module (conditionally compiled with "assembly" feature)
namespace assembly {
    // Pixel Bender assembly functionality
}

// Disassembly module
namespace disassembly {
    // Pixel Bender disassembly functionality
}

// Parser module (private)
namespace parser {
    // Pixel Bender parser functionality
    // Re-exported via pub use parser::*
}

// Re-export parser contents
using namespace parser;

} // namespace pixel_bender
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_PIXEL_BENDER_LIB_H
