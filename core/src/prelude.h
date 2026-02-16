// C++ translation of core/src/prelude.rs
// Common imports and type aliases

#ifndef RUFFLE_CORE_PRELUDE_H
#define RUFFLE_CORE_PRELUDE_H

// Display object imports
#include "../display_object.h"

// Matrix import
#include "../../render/src/matrix.h"

// Standard library imports
#include <cstdint>
#include <memory>

// SWF type imports
// These would come from the swf library
namespace swf {
    using CharacterId = uint16_t;
    // Color, Point, Rectangle, Twips would be defined in swf types
}

namespace ruffle {
namespace core {

/// A depth for a Flash display object in AVM1.
/// This is different than defined in `swf`; during execution, clips
/// created from SWF tags have their depth biased to negative numbers,
/// and clips can be dynamically switched by AS to depths in the range of 32-bits.
using Depth = int32_t;

} // namespace core
} // namespace ruffle

// Re-export commonly used types
namespace ruffle {
namespace core {
namespace prelude {

// Display object types
using DisplayObject = ruffle::core::display_object::DisplayObject;
using HitTestOptions = ruffle::core::display_object::HitTestOptions;
using TDisplayObject = ruffle::core::display_object::TDisplayObject;
using TDisplayObjectContainer = ruffle::core::display_object::TDisplayObjectContainer;

// Matrix type
using Matrix = ruffle::render::Matrix;

// SWF types
using CharacterId = swf::CharacterId;
// Color, Point, Rectangle, Twips would be re-exported from swf

} // namespace prelude
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_PRELUDE_H
