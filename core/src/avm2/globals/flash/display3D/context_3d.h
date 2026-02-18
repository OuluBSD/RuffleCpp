// C++ translation of core/src/avm2/globals/flash/display3D/context_3d.rs
// flash.display3D.Context3D native methods

#ifndef RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_CONTEXT3D_H
#define RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_CONTEXT3D_H

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

// Forward declarations
namespace ruffle {
namespace core {
namespace avm2 {
    template<typename GC>
    class Activation;
    template<typename GC>
    class Value;
    template<typename GC>
    class Error;
    template<typename GC>
    class Object;
}
}
}

namespace ruffle {
namespace render {
namespace backend {
    enum class BufferUsage;
    enum class Context3DBlendFactor;
    enum class Context3DCompareMode;
    enum class Context3DTextureFormat;
    enum class Context3DTriangleFace;
    enum class Context3DVertexBufferFormat;
    enum class Context3DWrapMode;
    enum class Context3DTextureFilter;
    enum class ProgramType;
    enum class Context3DProfile;
}
}
}

namespace swf {
    struct Rectangle;
}

namespace ruffle {
namespace core {
namespace avm2 {
namespace globals {
namespace flash_display3d_context3d {

/// Create an index buffer for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [num_indices: u32]
/// @return The created IndexBuffer3D, or Undefined on error
template<typename GC>
Value<GC> create_index_buffer(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Create a vertex buffer for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [num_vertices: u32, data32PerVertex: u32]
/// @return The created VertexBuffer3D, or Undefined on error
template<typename GC>
Value<GC> create_vertex_buffer(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Configure the back buffer for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [width: u32, height: u32, antiAlias: u32, enableDepthAndStencil: bool, wantsBestResolution: bool, wantsBestResolutionOnBrowserZoom: bool]
/// @return Undefined
template<typename GC>
Value<GC> configure_back_buffer(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set a vertex buffer at the specified index.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [index: u32, buffer: VertexBuffer3D, bufferOffset: u32, format: string]
/// @return Undefined
template<typename GC>
Value<GC> set_vertex_buffer_at(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Create a program for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args No arguments
/// @return The created Program3D, or Undefined on error
template<typename GC>
Value<GC> create_program(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the current program for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [program: Program3D]
/// @return Undefined
template<typename GC>
Value<GC> set_program(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Draw triangles using the current program and buffers.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [indexBuffer: IndexBuffer3D, firstIndex: u32, numTriangles: u32]
/// @return Undefined
template<typename GC>
Value<GC> draw_triangles(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Present the rendered frame.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args No arguments
/// @return Undefined
template<typename GC>
Value<GC> present(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Get the Context3D profile.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args No arguments
/// @return The profile string, or Undefined on error
template<typename GC>
Value<GC> get_profile(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the culling mode for triangles.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [culling: string ("none", "back", "front", "frontAndBack")]
/// @return Undefined
template<typename GC>
Value<GC> set_culling(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set program constants from a Matrix3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [programType: string, firstRegister: u32, matrix: Matrix3D, transposedMatrix: bool]
/// @return Undefined
template<typename GC>
Value<GC> set_program_constants_from_matrix(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set program constants from a vector of floats.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [programType: string, firstRegister: u32, vector: Vector.<Number>, numRegisters: i32]
/// @return Undefined
template<typename GC>
Value<GC> set_program_constants_from_vector(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Clear the render target.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [red: Number, green: Number, blue: Number, alpha: Number, depth: Number, stencil: u32, mask: u32]
/// @return Undefined
template<typename GC>
Value<GC> clear(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Create a texture for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [width: i32, height: i32, format: string, optimizeForRenderToTexture: bool, streamingLevels: i32]
/// @return The created Texture, or Undefined on error
template<typename GC>
Value<GC> create_texture(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Create a rectangle texture for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [width: i32, height: i32, format: string, optimizeForRenderToTexture: bool]
/// @return The created RectangleTexture, or Undefined on error
template<typename GC>
Value<GC> create_rectangle_texture(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Create a cube texture for the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [size: i32, format: string, optimizeForRenderToTexture: bool, streamingLevels: i32]
/// @return The created CubeTexture, or Undefined on error
template<typename GC>
Value<GC> create_cube_texture(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set a texture at the specified sampler index.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [sampler: i32, texture: Texture, cube: bool]
/// @return Undefined
template<typename GC>
Value<GC> set_texture_at(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the color mask for rendering.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [red: bool, green: bool, blue: bool, alpha: bool]
/// @return Undefined
template<typename GC>
Value<GC> set_color_mask(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the depth test configuration.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [depthMask: bool, passCompareMode: string]
/// @return Undefined
template<typename GC>
Value<GC> set_depth_test(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the blend factors for rendering.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [sourceFactor: string, destinationFactor: string]
/// @return Undefined
template<typename GC>
Value<GC> set_blend_factors(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the render target to a texture.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [texture: Texture, enableDepthAndStencil: bool, antiAlias: u32, surfaceSelector: u32, colorOutputIndex: u32]
/// @return Undefined
template<typename GC>
Value<GC> set_render_to_texture(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the render target to the back buffer.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args No arguments
/// @return Undefined
template<typename GC>
Value<GC> set_render_to_back_buffer(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the sampler state at the specified index.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [sampler: i32, wrap: string, filter: string, mipfilter: string]
/// @return Undefined
template<typename GC>
Value<GC> set_sampler_state_at(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Set the scissor rectangle for rendering.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args Arguments: [rectangle: Rectangle]
/// @return Undefined
template<typename GC>
Value<GC> set_scissor_rectangle(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

/// Dispose the Context3D.
///
/// @param activation The current activation
/// @param this The Context3D object
/// @param args No arguments
/// @return Undefined
template<typename GC>
Value<GC> dispose(Activation<GC>* activation, Value<GC> this_, const std::vector<Value<GC>>& args);

} // namespace flash_display3d_context3d
} // namespace globals
} // namespace avm2
} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_AVM2_GLOBALS_FLASH_DISPLAY3D_CONTEXT3D_H
