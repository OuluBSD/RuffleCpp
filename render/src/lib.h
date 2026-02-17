// C++ translation of render/src/lib.rs
// Render module exports

#ifndef RUFFLE_RENDER_LIB_H
#define RUFFLE_RENDER_LIB_H

// Module declarations
namespace ruffle {
namespace render {

// Forward declare all render submodules
namespace atf { }
namespace backend { }
namespace bitmap { }
namespace blend { }
namespace error { }
namespace filters { }
namespace lines { }
namespace matrix { }
namespace matrix3d { }
namespace perspective_projection { }
namespace pixel_bender_support { }
namespace shader_source { }
namespace shape_utils { }
namespace transform { }
namespace utils { }
namespace commands { }
namespace quality { }

#ifdef RUFFLE_FEATURE_TESSELLATOR
namespace tessellator { }
#endif

} // namespace render
} // namespace ruffle

// Re-export pixel_bender
#include "../pixel_bender/src/lib.h"

namespace ruffle {
namespace render {

using namespace pixel_bender;

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_LIB_H
