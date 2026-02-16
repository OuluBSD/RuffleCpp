// C++ translation of core/src/binary_data.rs
// Binary data handling

#ifndef RUFFLE_CORE_BINARY_DATA_H
#define RUFFLE_CORE_BINARY_DATA_H

#include <memory>
#include <vector>
#include <cstdint>

// Forward declarations
namespace swf {
    struct DefineBinaryData;
}

namespace ruffle {
namespace common {
namespace tag_utils {
    class SwfMovie;
    class SwfSlice;
}
}
}

namespace ruffle {
namespace core {

/// Binary data wrapper
class BinaryData {
private:
    std::shared_ptr<ruffle::common::tag_utils::SwfSlice> data;

public:
    /// Create BinaryData from SWF tag
    static BinaryData from_swf_tag(
        std::shared_ptr<ruffle::common::tag_utils::SwfMovie> movie,
        const swf::DefineBinaryData& tag
    );

    /// Convert to byte vector
    std::vector<uint8_t> to_vec() const;

    /// Get the underlying SwfSlice
    const std::shared_ptr<ruffle::common::tag_utils::SwfSlice>& get_slice() const {
        return data;
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_BINARY_DATA_H
