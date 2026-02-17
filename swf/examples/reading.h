// C++ translation of swf/examples/reading.rs
// SWF reading example

#ifndef RUFFLE_SWF_EXAMPLES_READING_H
#define RUFFLE_SWF_EXAMPLES_READING_H

#include <string>
#include <filesystem>

// Forward declarations
namespace swf {
    struct Swf;
}

namespace ruffle {
namespace swf {
namespace examples {

/// Read and display information from an SWF file
/// This is an example function demonstrating SWF parsing
void read_swf_example(const std::filesystem::path& swf_path);

} // namespace examples
} // namespace swf
} // namespace ruffle

#endif // RUFFLE_SWF_EXAMPLES_READING_H
