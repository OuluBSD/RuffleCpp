// C++ translation of render/wgpu/src/surface/commands.rs
// WGPU surface command execution for rendering

#ifndef RUFFLE_RENDER_WGPU_SURFACE_COMMANDS_H
#define RUFFLE_RENDER_WGPU_SURFACE_COMMANDS_H

#include "../backend.h"
#include "../blend.h"
#include "../mesh.h"
#include "../buffer_builder.h"
#include "../buffer_pool.h"
#include "../dynamic_transforms.h"
#include "../../src/shape_utils.h"
#include "../../../core/src/bitmap/operations.h"

#include <wgpu/wgpu.hpp>
#include <vector>
#include <memory>
#include <cstdint>

namespace ruffle {
namespace render {
namespace wgpu {
namespace surface {
namespace commands {

// Forward declarations
class Surface;
class Pipelines;
class Descriptors;
class MaskState;
class Transforms;

/// Draw command types for rendering
enum class DrawCommandType {
    RenderBitmap,
    RenderTexture,
    RenderShape,
    DrawRect,
    DrawLine,
    DrawLineRect,
    DrawBezier,
    RenderColorMatrix,
    RenderDistortionMap,
    RenderConvolutionFilter,
    RenderDisplacementMap,
    RenderBlendShader,
    UpdateTexture,
    CopyTexture,
    RenderGradient,
    RenderStencil,
    Clear
};

/// Draw command for bitmap rendering
struct RenderBitmapCommand {
    BitmapHandle bitmap;
    uint32_t transform_buffer_index;
    bool smoothing;
    BlendMode blend_mode;
    bool render_stage3d;
};

/// Draw command for texture rendering
struct RenderTextureCommand {
    wgpu::Texture texture;
    std::vector<Bind> binds;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for shape rendering
struct RenderShapeCommand {
    ShapeHandle shape;
    uint32_t transform_buffer_index;
};

/// Draw command for rectangle drawing
struct DrawRectCommand {
    uint32_t transform_buffer_index;
};

/// Draw command for line drawing
struct DrawLineCommand {
    uint32_t transform_buffer_index;
};

/// Draw command for line rectangle drawing
struct DrawLineRectCommand {
    uint32_t transform_buffer_index;
};

/// Draw command for bezier curve drawing
struct DrawBezierCommand {
    uint32_t transform_buffer_index;
};

/// Draw command for color matrix filter
struct RenderColorMatrixCommand {
    wgpu::Texture texture;
    ColorMatrixTransform transform;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for distortion map filter
struct RenderDistortionMapCommand {
    wgpu::Texture texture;
    wgpu::Texture distortion_map;
    DistortionMapParams params;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for convolution filter
struct RenderConvolutionFilterCommand {
    wgpu::Texture texture;
    ConvolutionFilterParams params;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for displacement map filter
struct RenderDisplacementMapCommand {
    wgpu::Texture texture;
    wgpu::Texture displacement_map;
    DisplacementMapParams params;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for blend shader
struct RenderBlendShaderCommand {
    wgpu::Texture texture;
    PixelBenderShaderHandle shader;
    std::vector<wgpu::Texture> inputs;
    BlendShaderParams params;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for texture update
struct UpdateTextureCommand {
    BitmapHandle bitmap;
    wgpu::Texture texture;
};

/// Draw command for texture copy
struct CopyTextureCommand {
    wgpu::Texture source;
    wgpu::Texture destination;
    CopyRect source_rect;
    CopyRect dest_rect;
};

/// Draw command for gradient rendering
struct RenderGradientCommand {
    Gradient gradient;
    Matrix matrix;
    ColorTransform color_transform;
    uint32_t transform_buffer_index;
    BlendMode blend_mode;
};

/// Draw command for stencil operations
struct RenderStencilCommand {
    StencilOperation operation;
    uint32_t reference;
    uint32_t mask;
};

/// Draw command for clearing
struct ClearCommand {
    Color color;
};

/// Union of all draw command types
struct DrawCommand {
    DrawCommandType type;
    
    union {
        RenderBitmapCommand render_bitmap;
        RenderTextureCommand render_texture;
        RenderShapeCommand render_shape;
        DrawRectCommand draw_rect;
        DrawLineCommand draw_line;
        DrawLineRectCommand draw_line_rect;
        DrawBezierCommand draw_bezier;
        RenderColorMatrixCommand render_color_matrix;
        RenderDistortionMapCommand render_distortion_map;
        RenderConvolutionFilterCommand render_convolution_filter;
        RenderDisplacementMapCommand render_displacement_map;
        RenderBlendShaderCommand render_blend_shader;
        UpdateTextureCommand update_texture;
        CopyTextureCommand copy_texture;
        RenderGradientCommand render_gradient;
        RenderStencilCommand render_stencil;
        ClearCommand clear;
    };
    
    DrawCommand() : type(DrawCommandType::Clear) {}
    ~DrawCommand() {}  // Note: Requires manual cleanup due to union
};

/// Command renderer for executing draw commands
class CommandRenderer {
public:
    /// Create new command renderer
    /// 
    /// @param pipelines Rendering pipelines
    /// @param descriptors WGPU descriptors
    /// @param dynamic_transforms Dynamic transform data
    /// @param render_pass WGPU render pass
    /// @param num_masks Current mask nesting level
    /// @param mask_state Current mask state
    /// @param needs_stencil Whether stencil buffer is needed
    CommandRenderer(
        Pipelines* pipelines,
        Descriptors* descriptors,
        DynamicTransforms* dynamic_transforms,
        wgpu::RenderPass render_pass,
        uint32_t num_masks,
        MaskState mask_state,
        bool needs_stencil
    );
    
    /// Execute a draw command
    /// 
    /// @param command The draw command to execute
    void execute(const DrawCommand& command);
    
    /// Render a bitmap
    void render_bitmap(
        const BitmapHandle& bitmap,
        uint32_t transform_buffer_index,
        bool smoothing,
        BlendMode blend_mode,
        bool render_stage3d
    );
    
    /// Render a texture
    void render_texture(
        uint32_t transform_buffer_index,
        const std::vector<Bind>& binds,
        BlendMode blend_mode
    );
    
    /// Render a shape
    void render_shape(ShapeHandle shape, uint32_t transform_buffer_index);
    
    /// Draw a rectangle
    void draw_rect(uint32_t transform_buffer_index);
    
    /// Draw lines
    template<bool is_rect>
    void draw_lines(uint32_t transform_buffer_index);
    
    /// Draw bezier curves
    void draw_bezier(uint32_t transform_buffer_index);
    
    /// Render color matrix filter
    void render_color_matrix(
        wgpu::Texture texture,
        const ColorMatrixTransform& transform,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Render distortion map filter
    void render_distortion_map(
        wgpu::Texture texture,
        wgpu::Texture distortion_map,
        const DistortionMapParams& params,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Render convolution filter
    void render_convolution_filter(
        wgpu::Texture texture,
        const ConvolutionFilterParams& params,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Render displacement map filter
    void render_displacement_map(
        wgpu::Texture texture,
        wgpu::Texture displacement_map,
        const DisplacementMapParams& params,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Render blend shader
    void render_blend_shader(
        wgpu::Texture texture,
        PixelBenderShaderHandle shader,
        const std::vector<wgpu::Texture>& inputs,
        const BlendShaderParams& params,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Update texture from bitmap
    void update_texture(const BitmapHandle& bitmap, wgpu::Texture texture);
    
    /// Copy texture region
    void copy_texture(
        wgpu::Texture source,
        wgpu::Texture destination,
        const CopyRect& source_rect,
        const CopyRect& dest_rect
    );
    
    /// Render gradient
    void render_gradient(
        const Gradient& gradient,
        const Matrix& matrix,
        const ColorTransform& color_transform,
        uint32_t transform_buffer_index,
        BlendMode blend_mode
    );
    
    /// Execute stencil operation
    void render_stencil(StencilOperation operation, uint32_t reference, uint32_t mask);
    
    /// Clear render target
    void clear(const Color& color);

private:
    Pipelines* pipelines_;
    Descriptors* descriptors_;
    DynamicTransforms* dynamic_transforms_;
    wgpu::RenderPass render_pass_;
    uint32_t num_masks_;
    MaskState mask_state_;
    bool needs_stencil_;
    
    /// Apply mask state before rendering
    void apply_mask_state();
    
    /// Set blend mode for rendering
    void set_blend_mode(BlendMode blend_mode, RenderTargetMode target_mode);
    
    /// Get transform data from buffer
    const Transforms& get_transform(uint32_t index) const;
    
    /// Render bitmap with trivial blend
    template<typename BlendType>
    void render_bitmap_with_blend(
        const BitmapHandle& bitmap,
        const Transforms& transforms,
        bool smoothing,
        RenderTargetMode target_mode
    );
    
    /// Render bitmap with complex blend
    template<typename BlendType>
    void render_bitmap_complex_blend(
        const BitmapHandle& bitmap,
        const Transforms& transforms,
        bool smoothing,
        RenderTargetMode target_mode
    );
};

} // namespace commands
} // namespace surface
} // namespace wgpu
} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_WGPU_SURFACE_COMMANDS_H
