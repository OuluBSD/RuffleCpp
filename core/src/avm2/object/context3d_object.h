// C++ translation of core/src/avm2/object/context3d_object.rs
//! Object representation for Context3D objects

#ifndef RUFFLE_CORE_AVM2_OBJECT_CONTEXT3D_OBJECT_H
#define RUFFLE_CORE_AVM2_OBJECT_CONTEXT3D_OBJECT_H

#include <memory>
#include <cstdint>
#include <vector>
#include <optional>
#include <cell>
#include <format>

// Forward declarations
namespace ruffle {
namespace core {
    template<typename GC> class Activation;
    template<typename GC> class UpdateContext;
    template<typename GC> class ScriptObjectData;
    template<typename GC> class TObject;
    template<typename GC> class Gc;
    template<typename GC> class GcWeak;
    template<typename GC> class ClassObject;
    template<typename GC> class Object;
    template<typename GC> class Value;
    template<typename GC> class Error;

    namespace bitmap {
        namespace bitmap_data {
            template<typename GC> class BitmapRawData;
        }
    }

    namespace context {
        template<typename GC> class RenderContext;
    }

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
            class Context3D;
            class Context3DCommand;
            class Texture;
            class IndexBuffer;
            class VertexBuffer;
            class ShaderModule;
        }
    }

    namespace swf {
        template<typename T> struct Rectangle;
        struct Twips;
    }

    namespace avm2 {
        namespace object {
            template<typename GC> class Program3DObject;
            template<typename GC> class TextureObject;
            template<typename GC> class IndexBuffer3DObject;
            template<typename GC> class VertexBuffer3DObject;
            template<typename GC> class Stage3DObject;
        }
    }
}
}

namespace ruffle {
namespace core {
namespace avm2 {

/// Weak reference to Context3DObject data
template<typename GC>
class Context3DObjectWeak;

/// Data structure for Context3DObject
/// Uses HasPrefixField pattern - base must be first member
template<typename GC>
class Context3DData {
public:
    Context3DData() = default;

    // Base script object - must be first for HasPrefixField pattern
    ScriptObjectData<GC> base;

    /// Render context (Context3D instance)
    std::optional<std::unique_ptr<render::backend::Context3D>> render_context;

    /// Stage3D object that owns this context
    object::Stage3DObject<GC> stage3d;

    // Friend classes for accessing private members
    friend class Context3DObject<GC>;
    friend class Context3DObjectWeak<GC>;
};

/// Context3DObject - AVM2 Context3D object representation
/// Represents a Context3D instance in ActionScript 3
template<typename GC>
class Context3DObject {
public:
    using DataType = Context3DData<GC>;
    using WeakType = Context3DObjectWeak<GC>;

    Context3DObject() = default;
    explicit Context3DObject(Gc<GC, DataType> data) : data_(data) {}

    /// Create a Context3DObject from a context
    ///
    /// @param activation The current activation context
    /// @param context The Context3D implementation
    /// @param stage3d The Stage3D object that owns this context
    /// @return New Context3DObject as Object
    static Object<GC> from_context(
        Activation<GC>* activation,
        std::unique_ptr<render::backend::Context3D> context,
        object::Stage3DObject<GC> stage3d
    );

    /// Get the Stage3D object
    object::Stage3DObject<GC> stage3d() const {
        return data_->stage3d;
    }

    /// Execute a function with the Context3D instance
    /// Temporarily takes ownership to prevent reentrancy
    ///
    /// @param f Function to execute with Context3D
    /// @return Result of the function
    template<typename R>
    R with_context_3d(std::function<R(render::backend::Context3D&)> f) {
        if (!data_->render_context.has_value()) {
            // Error: Context3D is missing or already in use
            throw std::runtime_error("Context3D is missing or already in use");
        }
        auto& ctx = *data_->render_context.value();
        return f(*ctx);
    }

    /// Configure the back buffer
    ///
    /// @param width Back buffer width
    /// @param height Back buffer height
    /// @param anti_alias Anti-aliasing level
    /// @param depth_and_stencil Whether to enable depth and stencil buffers
    /// @param wants_best_resolution Request best resolution
    /// @param wants_best_resolution_on_browser_zoom Request best resolution on browser zoom
    void configure_back_buffer(
        uint32_t width,
        uint32_t height,
        uint32_t anti_alias,
        bool depth_and_stencil,
        bool wants_best_resolution,
        bool wants_best_resolution_on_browser_zoom
    );

    /// Create an index buffer
    ///
    /// @param num_indices Number of indices
    /// @param activation The current activation context
    /// @return IndexBuffer3DObject as Value
    Value<GC> create_index_buffer(
        uint32_t num_indices,
        Activation<GC>* activation
    );

    /// Create a texture
    ///
    /// @param width Texture width
    /// @param height Texture height
    /// @param format Texture format
    /// @param optimize_for_render_to_texture Optimize for render-to-texture
    /// @param streaming_levels Number of streaming levels
    /// @param class_obj The texture class object
    /// @param activation The current activation context
    /// @return TextureObject as Value, or Error on failure
    Result<Value<GC>, Error<GC>> create_texture(
        uint32_t width,
        uint32_t height,
        render::backend::Context3DTextureFormat format,
        bool optimize_for_render_to_texture,
        uint32_t streaming_levels,
        ClassObject<GC> class_obj,
        Activation<GC>* activation
    );

    /// Create a vertex buffer
    ///
    /// @param num_vertices Number of vertices
    /// @param data_32_per_vertex Number of 32-bit values per vertex
    /// @param usage Buffer usage hint
    /// @param activation The current activation context
    /// @return VertexBuffer3DObject as Value
    Value<GC> create_vertex_buffer(
        uint32_t num_vertices,
        uint8_t data_32_per_vertex,
        render::backend::BufferUsage usage,
        Activation<GC>* activation
    );

    /// Upload vertex buffer data
    ///
    /// @param buffer The vertex buffer
    /// @param data The data to upload
    /// @param start_vertex Starting vertex index
    /// @param data32_per_vertex Number of 32-bit values per vertex
    void upload_vertex_buffer_data(
        object::VertexBuffer3DObject<GC> buffer,
        const std::vector<uint8_t>& data,
        size_t start_vertex,
        uint8_t data32_per_vertex
    );

    /// Upload index buffer data
    ///
    /// @param buffer The index buffer
    /// @param data The data to upload
    /// @param start_offset Starting offset in bytes
    void upload_index_buffer_data(
        object::IndexBuffer3DObject<GC> buffer,
        const std::vector<uint8_t>& data,
        size_t start_offset
    );

    /// Set vertex buffer at an index
    ///
    /// @param index The vertex stream index
    /// @param buffer Optional vertex buffer and format
    /// @param buffer_offset Offset in the buffer
    void set_vertex_buffer_at(
        uint32_t index,
        std::optional<std::pair<object::VertexBuffer3DObject<GC>, render::backend::Context3DVertexBufferFormat>> buffer,
        uint32_t buffer_offset
    );

    /// Create a program
    ///
    /// @param activation The current activation context
    /// @return Program3DObject as Value
    Value<GC> create_program(Activation<GC>* activation);

    /// Upload shaders to a program
    ///
    /// @param program The program
    /// @param vertex_shader_agal Vertex shader AGAL bytecode
    /// @param fragment_shader_agal Fragment shader AGAL bytecode
    void upload_shaders(
        object::Program3DObject<GC> program,
        std::vector<uint8_t> vertex_shader_agal,
        std::vector<uint8_t> fragment_shader_agal
    );

    /// Set the current program
    ///
    /// @param program The program to set, or nullopt to clear
    void set_program(std::optional<object::Program3DObject<GC>> program);

    /// Draw triangles
    ///
    /// @param index_buffer The index buffer
    /// @param first_index First index to draw
    /// @param num_triangles Number of triangles (-1 for all)
    void draw_triangles(
        object::IndexBuffer3DObject<GC> index_buffer,
        uint32_t first_index,
        int32_t num_triangles
    );

    /// Set program constants from a matrix
    ///
    /// @param program_type Program type (vertex or fragment)
    /// @param first_register First register index
    /// @param matrix_raw_data_column_major Matrix data in column-major order
    void set_program_constants_from_matrix(
        render::backend::ProgramType program_type,
        uint32_t first_register,
        std::vector<float> matrix_raw_data_column_major
    );

    /// Set culling mode
    ///
    /// @param face The face culling mode
    void set_culling(render::backend::Context3DTriangleFace face);

    /// Set blend factors
    ///
    /// @param source_factor Source blend factor
    /// @param destination_factor Destination blend factor
    void set_blend_factors(
        render::backend::Context3DBlendFactor source_factor,
        render::backend::Context3DBlendFactor destination_factor
    );

    /// Set render target to texture
    ///
    /// @param texture The render target texture
    /// @param enable_depth_and_stencil Enable depth and stencil buffers
    /// @param anti_alias Anti-aliasing level
    /// @param surface_selector Surface selector for cube maps
    void set_render_to_texture(
        std::shared_ptr<render::backend::Texture> texture,
        bool enable_depth_and_stencil,
        uint32_t anti_alias,
        uint32_t surface_selector
    );

    /// Set render target to back buffer
    void set_render_to_back_buffer();

    /// Present the rendered frame
    void present();

    /// Render to the screen as part of Ruffle rendering
    ///
    /// @param context The render context
    void render(context::RenderContext<GC>* context);

    /// Set clear color and buffers
    ///
    /// @param red Clear color red (0-1)
    /// @param green Clear color green (0-1)
    /// @param blue Clear color blue (0-1)
    /// @param alpha Clear color alpha (0-1)
    /// @param depth Clear depth value
    /// @param stencil Clear stencil value
    /// @param mask Clear mask
    void set_clear(
        double red,
        double green,
        double blue,
        double alpha,
        double depth,
        uint32_t stencil,
        uint32_t mask
    );

    /// Copy bitmap data to texture
    ///
    /// @param source Source bitmap data
    /// @param dest Destination texture
    /// @param layer Cubemap layer index
    void copy_bitmapdata_to_texture(
        const bitmap::bitmap_data::BitmapRawData<GC>* source,
        std::shared_ptr<render::backend::Texture> dest,
        uint32_t layer
    );

    /// Copy pixels to texture
    ///
    /// @param source Source pixel data
    /// @param dest Destination texture
    /// @param layer Cubemap layer index
    void copy_pixels_to_texture(
        std::vector<uint8_t> source,
        std::shared_ptr<render::backend::Texture> dest,
        uint32_t layer
    );

    /// Set texture at sampler
    ///
    /// @param sampler Sampler index
    /// @param texture Optional texture
    /// @param cube Whether this is a cube map
    void set_texture_at(
        uint32_t sampler,
        std::optional<std::shared_ptr<render::backend::Texture>> texture,
        bool cube
    );

    /// Set color mask
    ///
    /// @param red Enable red channel
    /// @param green Enable green channel
    /// @param blue Enable blue channel
    /// @param alpha Enable alpha channel
    void set_color_mask(bool red, bool green, bool blue, bool alpha);

    /// Set depth test
    ///
    /// @param depth_mask Enable depth writes
    /// @param pass_compare_mode Depth comparison mode
    void set_depth_test(
        bool depth_mask,
        render::backend::Context3DCompareMode pass_compare_mode
    );

    /// Create a cube texture
    ///
    /// @param size Cube face size
    /// @param format Texture format
    /// @param optimize_for_render_to_texture Optimize for render-to-texture
    /// @param streaming_levels Number of streaming levels
    /// @param activation The current activation context
    /// @return CubeTextureObject as Value, or Error on failure
    Result<Value<GC>, Error<GC>> create_cube_texture(
        uint32_t size,
        render::backend::Context3DTextureFormat format,
        bool optimize_for_render_to_texture,
        uint32_t streaming_levels,
        Activation<GC>* activation
    );

    /// Set sampler state
    ///
    /// @param sampler Sampler index
    /// @param wrap Wrap mode
    /// @param filter Filter mode
    void set_sampler_state_at(
        uint32_t sampler,
        render::backend::Context3DWrapMode wrap,
        render::backend::Context3DTextureFilter filter
    );

    /// Set scissor rectangle
    ///
    /// @param rect Optional scissor rectangle
    void set_scissor_rectangle(std::optional<swf::Rectangle<int32_t>> rect);

    /// Get the base script object data (for TObject trait)
    Gc<GC, ScriptObjectData<GC>> gc_base() const {
        // Uses HasPrefixField pattern - base is at the start of the struct
        return data_->base;
    }

private:
    Gc<GC, DataType> data_;
};

/// Weak reference to Context3DObject
template<typename GC>
class Context3DObjectWeak {
public:
    Context3DObjectWeak() = default;
    explicit Context3DObjectWeak(GcWeak<GC, Context3DData<GC>> data)
        : data_(data) {}

private:
    GcWeak<GC, Context3DData<GC>> data_;
};

// Template method implementations

template<typename GC>
Object<GC> Context3DObject<GC>::from_context(
    Activation<GC>* activation,
    std::unique_ptr<render::backend::Context3D> context,
    object::Stage3DObject<GC> stage3d
) {
    auto class_obj = activation->avm2()->classes()->context3d;
    auto base = ScriptObjectData<GC>::create(class_obj);
    auto data = Gc<GC, DataType>::create(
        activation->gc(),
        DataType{
            base,
            std::make_optional(std::move(context)),
            stage3d
        }
    );
    return Object<GC>(Context3DObject<GC>(data));
}

template<typename GC>
void Context3DObject<GC>::configure_back_buffer(
    uint32_t width,
    uint32_t height,
    uint32_t anti_alias,
    bool depth_and_stencil,
    bool wants_best_resolution,
    bool wants_best_resolution_on_browser_zoom
) {
    with_context_3d([&](render::backend::Context3D& ctx) {
        ctx.process_command(render::backend::Context3DCommand::configure_back_buffer(
            width,
            height,
            anti_alias,
            depth_and_stencil,
            wants_best_resolution,
            wants_best_resolution_on_browser_zoom
        ));
    });
}

template<typename GC>
Value<GC> Context3DObject<GC>::create_index_buffer(
    uint32_t num_indices,
    Activation<GC>* activation
) {
    auto index_buffer = with_context_3d([&](render::backend::Context3D& ctx) {
        return ctx.create_index_buffer(
            render::backend::BufferUsage::StaticDraw,
            num_indices
        );
    });

    return Value<GC>::from_object(object::IndexBuffer3DObject<GC>::from_handle(
        activation,
        *this,
        std::move(index_buffer)
    ));
}

} // namespace avm2
} // namespace core
} // namespace ruffle

// Debug formatter specialization
template<typename GC>
struct std::formatter<ruffle::core::avm2::Context3DObject<GC>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const ruffle::core::avm2::Context3DObject<GC>& obj, FormatContext& ctx) {
        return std::format_to(ctx.out(), "Context3DObject(ptr={})",
            static_cast<const void*>(obj.data_.get()));
    }
};

#endif // RUFFLE_CORE_AVM2_OBJECT_CONTEXT3D_OBJECT_H
