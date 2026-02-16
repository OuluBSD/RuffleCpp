// C++ translation of render/src/commands.rs
// Rendering command types and command list management

#ifndef RUFFLE_RENDER_COMMANDS_H
#define RUFFLE_RENDER_COMMANDS_H

#include <vector>
#include <memory>
#include <cstdint>
#include <variant>

// Forward declarations
namespace ruffle {
namespace render {
    class BitmapHandle;
    class PixelBenderShaderHandle;
    struct Matrix;
    struct Transform;
    class ShapeHandle;
    enum class PixelSnapping;
}
namespace swf {
    enum class BlendMode;
    struct Color;
}
}

namespace ruffle {
namespace render {

/// Forward declaration for CommandList
class CommandList;

/// Holds either a normal BlendMode, or the shader for BlendMode.SHADER.
///
/// We cannot store the `PixelBenderShaderHandle` directly in `ExtendedBlendMode`,
/// since we need to remember the shader even if the blend mode is changed
/// to something else (so that the shader will still be used if we switch back)
class RenderBlendMode {
public:
    /// Blend mode variant type
    enum class Type {
        Builtin,
        Shader,
    };

private:
    Type type_;
    swf::BlendMode builtin_mode_;
    PixelBenderShaderHandle shader_;

public:
    /// Create a builtin blend mode
    explicit RenderBlendMode(swf::BlendMode mode)
        : type_(Type::Builtin)
        , builtin_mode_(mode)
    {}

    /// Create a shader blend mode
    explicit RenderBlendMode(PixelBenderShaderHandle shader)
        : type_(Type::Shader)
        , shader_(std::move(shader))
    {}

    /// Get the blend mode type
    Type type() const { return type_; }

    /// Get the builtin mode if applicable
    const swf::BlendMode* builtin() const {
        return type_ == Type::Builtin ? &builtin_mode_ : nullptr;
    }

    /// Get the shader if applicable
    const PixelBenderShaderHandle* shader() const {
        return type_ == Type::Shader ? &shader_ : nullptr;
    }
};

/// Command enumeration for rendering operations
/// Each variant represents a different type of rendering command
class Command {
public:
    /// Command variant types
    enum class Type {
        RenderBitmap,
        RenderStage3D,
        RenderShape,
        RenderAlphaMask,
        DrawRect,
        DrawLine,
        DrawLineRect,
        PushMask,
        ActivateMask,
        DeactivateMask,
        PopMask,
        Blend,
    };

private:
    Type type_;

    // Storage for each command type
    struct RenderBitmapData {
        BitmapHandle bitmap;
        Transform transform;
        bool smoothing;
        PixelSnapping pixel_snapping;
    };

    struct RenderStage3DData {
        BitmapHandle bitmap;
        Transform transform;
    };

    struct RenderShapeData {
        ShapeHandle shape;
        Transform transform;
    };

    struct RenderAlphaMaskData {
        CommandList maskee_commands;
        CommandList mask_commands;
    };

    struct DrawRectData {
        swf::Color color;
        Matrix matrix;
    };

    struct DrawLineData {
        swf::Color color;
        Matrix matrix;
    };

    struct DrawLineRectData {
        swf::Color color;
        Matrix matrix;
    };

    struct BlendData {
        CommandList commands;
        RenderBlendMode blend_mode;
    };

    std::optional<RenderBitmapData> render_bitmap_;
    std::optional<RenderStage3DData> render_stage3d_;
    std::optional<RenderShapeData> render_shape_;
    std::optional<RenderAlphaMaskData> render_alpha_mask_;
    std::optional<DrawRectData> draw_rect_;
    std::optional<DrawLineData> draw_line_;
    std::optional<DrawLineRectData> draw_line_rect_;
    std::optional<BlendData> blend_;

public:
    /// Create a RenderBitmap command
    static Command render_bitmap(BitmapHandle bitmap, Transform transform,
                                  bool smoothing, PixelSnapping pixel_snapping) {
        Command cmd;
        cmd.type_ = Type::RenderBitmap;
        cmd.render_bitmap_ = RenderBitmapData{
            std::move(bitmap), std::move(transform), smoothing, pixel_snapping
        };
        return cmd;
    }

    /// Create a RenderStage3D command
    static Command render_stage3d(BitmapHandle bitmap, Transform transform) {
        Command cmd;
        cmd.type_ = Type::RenderStage3D;
        cmd.render_stage3d_ = RenderStage3DData{std::move(bitmap), std::move(transform)};
        return cmd;
    }

    /// Create a RenderShape command
    static Command render_shape(ShapeHandle shape, Transform transform) {
        Command cmd;
        cmd.type_ = Type::RenderShape;
        cmd.render_shape_ = RenderShapeData{std::move(shape), std::move(transform)};
        return cmd;
    }

    /// Create a RenderAlphaMask command
    static Command render_alpha_mask(CommandList maskee_commands, CommandList mask_commands) {
        Command cmd;
        cmd.type_ = Type::RenderAlphaMask;
        cmd.render_alpha_mask_ = RenderAlphaMaskData{
            std::move(maskee_commands), std::move(mask_commands)
        };
        return cmd;
    }

    /// Create a DrawRect command
    static Command draw_rect(swf::Color color, Matrix matrix) {
        Command cmd;
        cmd.type_ = Type::DrawRect;
        cmd.draw_rect_ = DrawRectData{color, matrix};
        return cmd;
    }

    /// Create a DrawLine command
    static Command draw_line(swf::Color color, Matrix matrix) {
        Command cmd;
        cmd.type_ = Type::DrawLine;
        cmd.draw_line_ = DrawLineData{color, matrix};
        return cmd;
    }

    /// Create a DrawLineRect command
    static Command draw_line_rect(swf::Color color, Matrix matrix) {
        Command cmd;
        cmd.type_ = Type::DrawLineRect;
        cmd.draw_line_rect_ = DrawLineRectData{color, matrix};
        return cmd;
    }

    /// Create a PushMask command
    static Command push_mask() {
        Command cmd;
        cmd.type_ = Type::PushMask;
        return cmd;
    }

    /// Create an ActivateMask command
    static Command activate_mask() {
        Command cmd;
        cmd.type_ = Type::ActivateMask;
        return cmd;
    }

    /// Create a DeactivateMask command
    static Command deactivate_mask() {
        Command cmd;
        cmd.type_ = Type::DeactivateMask;
        return cmd;
    }

    /// Create a PopMask command
    static Command pop_mask() {
        Command cmd;
        cmd.type_ = Type::PopMask;
        return cmd;
    }

    /// Create a Blend command
    static Command blend(CommandList commands, RenderBlendMode blend_mode) {
        Command cmd;
        cmd.type_ = Type::Blend;
        cmd.blend_ = BlendData{std::move(commands), std::move(blend_mode)};
        return cmd;
    }

    /// Get the command type
    Type type() const { return type_; }

    /// Accessors for each command type
    const RenderBitmapData* as_render_bitmap() const { return render_bitmap_ ? &*render_bitmap_ : nullptr; }
    const RenderStage3DData* as_render_stage3d() const { return render_stage3d_ ? &*render_stage3d_ : nullptr; }
    const RenderShapeData* as_render_shape() const { return render_shape_ ? &*render_shape_ : nullptr; }
    const RenderAlphaMaskData* as_render_alpha_mask() const { return render_alpha_mask_ ? &*render_alpha_mask_ : nullptr; }
    const DrawRectData* as_draw_rect() const { return draw_rect_ ? &*draw_rect_ : nullptr; }
    const DrawLineData* as_draw_line() const { return draw_line_ ? &*draw_line_ : nullptr; }
    const DrawLineRectData* as_draw_line_rect() const { return draw_line_rect_ ? &*draw_line_rect_ : nullptr; }
    const BlendData* as_blend() const { return blend_ ? &*blend_ : nullptr; }
};

/// Abstract interface for handling rendering commands
/// This is the C++ equivalent of the Rust CommandHandler trait
class CommandHandler {
public:
    virtual ~CommandHandler() = default;

    virtual void render_bitmap(BitmapHandle bitmap, Transform transform,
                               bool smoothing, PixelSnapping pixel_snapping) = 0;

    virtual void render_stage3d(BitmapHandle bitmap, Transform transform) = 0;

    virtual void render_shape(ShapeHandle shape, Transform transform) = 0;

    virtual void render_alpha_mask(CommandList maskee_commands, CommandList mask_commands) = 0;

    virtual void draw_rect(swf::Color color, Matrix matrix) = 0;

    virtual void draw_line(swf::Color color, Matrix matrix) = 0;

    virtual void draw_line_rect(swf::Color color, Matrix matrix) = 0;

    virtual void push_mask() = 0;

    virtual void activate_mask() = 0;

    virtual void deactivate_mask() = 0;

    virtual void pop_mask() = 0;

    virtual void blend(CommandList commands, RenderBlendMode blend_mode) = 0;
};

/// A list of rendering commands
///
/// Holds either a normal BlendMode, or the shader for BlendMode.SHADER.
/// This is used to track nested maskers and discard drawing commands
/// of nested maskers, which Flash does not support.
class CommandList : public CommandHandler {
private:
    std::vector<Command> commands_;

    /// The number of mask regions in the process of being drawn.
    /// This is used to discard drawing commands of nested maskers, which Flash does not support.
    uint32_t maskers_in_progress_;

public:
    /// Default constructor
    CommandList() : maskers_in_progress_(0) {}

    /// Check if the command list is empty
    bool is_empty() const { return commands_.empty(); }

    /// Get the commands
    const std::vector<Command>& commands() const { return commands_; }

    /// Execute all commands in this list on the given handler
    void execute(CommandHandler& handler) {
        for (const auto& command : commands_) {
            switch (command.type()) {
                case Command::Type::RenderBitmap: {
                    const auto* data = command.as_render_bitmap();
                    if (data) {
                        handler.render_bitmap(data->bitmap, data->transform,
                                              data->smoothing, data->pixel_snapping);
                    }
                    break;
                }
                case Command::Type::RenderStage3D: {
                    const auto* data = command.as_render_stage3d();
                    if (data) {
                        handler.render_stage3d(data->bitmap, data->transform);
                    }
                    break;
                }
                case Command::Type::RenderShape: {
                    const auto* data = command.as_render_shape();
                    if (data) {
                        handler.render_shape(data->shape, data->transform);
                    }
                    break;
                }
                case Command::Type::RenderAlphaMask: {
                    const auto* data = command.as_render_alpha_mask();
                    if (data) {
                        handler.render_alpha_mask(data->maskee_commands, data->mask_commands);
                    }
                    break;
                }
                case Command::Type::DrawRect: {
                    const auto* data = command.as_draw_rect();
                    if (data) {
                        handler.draw_rect(data->color, data->matrix);
                    }
                    break;
                }
                case Command::Type::DrawLine: {
                    const auto* data = command.as_draw_line();
                    if (data) {
                        handler.draw_line(data->color, data->matrix);
                    }
                    break;
                }
                case Command::Type::DrawLineRect: {
                    const auto* data = command.as_draw_line_rect();
                    if (data) {
                        handler.draw_line_rect(data->color, data->matrix);
                    }
                    break;
                }
                case Command::Type::PushMask:
                    handler.push_mask();
                    break;
                case Command::Type::ActivateMask:
                    handler.activate_mask();
                    break;
                case Command::Type::DeactivateMask:
                    handler.deactivate_mask();
                    break;
                case Command::Type::PopMask:
                    handler.pop_mask();
                    break;
                case Command::Type::Blend: {
                    const auto* data = command.as_blend();
                    if (data) {
                        handler.blend(data->commands, data->blend_mode);
                    }
                    break;
                }
            }
        }
    }

    /// Check if we're currently drawing a mask
    bool drawing_mask() const { return maskers_in_progress_ > 0; }

    // CommandHandler interface implementation
    void render_bitmap(BitmapHandle bitmap, Transform transform,
                       bool smoothing, PixelSnapping pixel_snapping) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::render_bitmap(
                std::move(bitmap), std::move(transform), smoothing, pixel_snapping));
        }
    }

    void render_stage3d(BitmapHandle bitmap, Transform transform) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::render_stage3d(std::move(bitmap), std::move(transform)));
        }
    }

    void render_shape(ShapeHandle shape, Transform transform) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::render_shape(std::move(shape), std::move(transform)));
        }
    }

    void render_alpha_mask(CommandList maskee_commands, CommandList mask_commands) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::render_alpha_mask(
                std::move(maskee_commands), std::move(mask_commands)));
        }
    }

    void draw_rect(swf::Color color, Matrix matrix) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::draw_rect(color, matrix));
        }
    }

    void draw_line(swf::Color color, Matrix matrix) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::draw_line(color, matrix));
        }
    }

    void draw_line_rect(swf::Color color, Matrix matrix) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::draw_line_rect(color, matrix));
        }
    }

    void push_mask() override {
        if (maskers_in_progress_ == 0) {
            commands_.push_back(Command::push_mask());
        }
        maskers_in_progress_++;
    }

    void activate_mask() override {
        maskers_in_progress_--;
        if (maskers_in_progress_ == 0) {
            commands_.push_back(Command::activate_mask());
        }
    }

    void deactivate_mask() override {
        if (maskers_in_progress_ == 0) {
            commands_.push_back(Command::deactivate_mask());
        }
        maskers_in_progress_++;
    }

    void pop_mask() override {
        maskers_in_progress_--;
        if (maskers_in_progress_ == 0) {
            commands_.push_back(Command::pop_mask());
        }
    }

    void blend(CommandList commands, RenderBlendMode blend_mode) override {
        if (maskers_in_progress_ <= 1) {
            commands_.push_back(Command::blend(std::move(commands), std::move(blend_mode)));
        }
    }
};

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_COMMANDS_H
