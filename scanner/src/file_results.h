// C++ translation of scanner/src/file_results.rs
//! File results type.
//!
//! The `FileResults` type in this module is used to report results of a scan.

#ifndef RUFFLE_SCANNER_FILE_RESULTS_H
#define RUFFLE_SCANNER_FILE_RESULTS_H

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

// Forward declarations
namespace ruffle {
namespace swf {
    enum class Compression;
}
}

namespace ruffle {
namespace scanner {

/// AVM type detected in a SWF file
enum class AvmType {
    Avm1,
    Avm2
};

/// Compression type used in a SWF file
enum class Compression {
    None,
    Zlib,
    Lzma
};

/// Convert from swf::Compression to scanner Compression
inline Compression from_swf_compression(swf::Compression sc);

/// A particular step in the scanner process.
enum class Step {
    /// Nothing has been done yet.
    ///
    /// Usually this indicates a significant problem unrelated to Ruffle, or a
    /// scanner child process panic.
    Start,

    /// Reading of the file into memory and computing it's SHA256 hash.
    Read,

    /// Decompression of the file data into a SWF bytestream.
    Decompress,

    /// Parsing of the decompressed SWF.
    Parse,

    /// Execution of the SWF in Ruffle.
    Execute,

    /// Completion of all prior steps without error.
    Complete
};

/// The result of a single scan.
struct FileResults {
    /// The file name scanned (including path).
    std::string name;

    /// The SHA256 hash of the SWF file.
    std::vector<uint8_t> hash;

    /// How far we were able to process this particular SWF
    Step progress;

    /// How long testing took to complete
    uint128_t testing_time;

    /// The compressed length of the SWF file.
    std::optional<size_t> compressed_len;

    /// The uncompressed length of the SWF file.
    std::optional<int32_t> uncompressed_len;

    /// Any errors encountered while testing.
    std::optional<std::string> error;

    /// The compression type this SWF uses.
    std::optional<Compression> compression;

    /// The file format version of this SWF.
    std::optional<uint8_t> version;

    /// The stage size of this SWF.
    std::optional<std::string> stage_size;

    /// The frame rate of this SWF.
    std::optional<float> frame_rate;

    /// The number of frames this SWF claims to contain.
    std::optional<uint16_t> num_frames;

    /// Whether or not the SWF requests hardware-accelerated presentation.
    std::optional<bool> use_direct_blit;

    /// Whether or not the SWF requests hardware-accelerated compositing.
    std::optional<bool> use_gpu;

    /// Whether or not the SWF requests network access when ran locally.
    std::optional<bool> use_network_sandbox;

    /// The AVM type of the movie.
    std::optional<AvmType> vm_type;

    /// Default constructor
    FileResults() : FileResults("") {}

    /// Constructor with filename
    /// @param name The filename
    explicit FileResults(const std::string& name_)
        : name(name_),
          hash(),
          progress(Step::Start),
          testing_time(0),
          compressed_len(std::nullopt),
          uncompressed_len(std::nullopt),
          error(std::nullopt),
          compression(std::nullopt),
          version(std::nullopt),
          stage_size(std::nullopt),
          frame_rate(std::nullopt),
          num_frames(std::nullopt),
          use_direct_blit(std::nullopt),
          use_gpu(std::nullopt),
          use_network_sandbox(std::nullopt),
          vm_type(std::nullopt) {}

    /// Format hash as capital hex string
    /// @return Hex string representation of the hash
    std::string hash_to_hex() const;

    /// Parse hex string into hash bytes
    /// @param hex The hex string to parse
    /// @return Vector of bytes, or empty vector on error
    static std::vector<uint8_t> hex_to_hash(const std::string& hex);
};

} // namespace scanner
} // namespace ruffle

#endif // RUFFLE_SCANNER_FILE_RESULTS_H
