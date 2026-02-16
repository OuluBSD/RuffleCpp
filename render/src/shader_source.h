// C++ translation of render/src/shader_source.rs
// Shader source code constants

#ifndef RUFFLE_RENDER_SHADER_SOURCE_H
#define RUFFLE_RENDER_SHADER_SOURCE_H

#include <string_view>

namespace ruffle {
namespace render {

// Shader filter common WGSL code
// This is included from the shader_filter_common.wgsl file
inline constexpr const char* SHADER_FILTER_COMMON = R"(
// Shader filter common WGSL code
// Contents from shader_filter_common.wgsl will be placed here
)";

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_SHADER_SOURCE_H
