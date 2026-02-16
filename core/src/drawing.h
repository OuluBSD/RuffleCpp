// C++ translation of core/src/drawing.rs
// Drawing data structure for shape rendering

#ifndef RUFFLE_CORE_DRAWING_H
#define RUFFLE_CORE_DRAWING_H

#include <cstdint>
#include <vector>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace render {
    class RenderBackend;
    class BitmapHandle;
    class BitmapInfo;
    class BitmapSize;
    class Matrix;
    template<typename T>
    class Point;
    template<typename T>
    class Rectangle;
    namespace backend {
        using ShapeHandle = uint32_t;
    }
    namespace shape_utils {
        enum class FillRule;
        class DrawCommand;
        class DrawPath;
    }
}
namespace swf {
    class FillStyle;
    class LineStyle;
    template<typename T>
    class Point;
    template<typename T>
    class Rectangle;
    class Shape;
    enum class ShapeFlag;
}
template<typename T>
class RenderContext;
}

namespace ruffle {
namespace core {

using ShapeHandle = render::backend::ShapeHandle;
using FillRule = render::shape_utils::FillRule;
using DrawCommand = render::shape_utils::DrawCommand;

/// Internal fill data for drawing
struct DrawingFill {
    swf::FillStyle style;
    FillRule rule;
    std::vector<DrawCommand> commands;
};

/// Internal line data for drawing
struct DrawingLine {
    swf::LineStyle style;
    std::vector<DrawCommand> commands;
    bool is_closed;
};

/// Path type for drawing
enum class DrawingPathType {
    Fill,
    Line,
};

/// Internal path data for drawing
struct DrawingPath {
    DrawingPathType type;
    std::optional<DrawingFill> fill;
    std::optional<DrawingLine> line;
    
    static DrawingPath fill(DrawingFill f) {
        return {DrawingPathType::Fill, std::move(f), std::nullopt, false};
    }
    
    static DrawingPath line(DrawingLine l) {
        return {DrawingPathType::Line, std::nullopt, std::move(l), false};
    }
};

/// Drawing data structure for shape rendering
class Drawing {
private:
    std::optional<ShapeHandle> render_handle;
    swf::Rectangle<swf::Twips> shape_bounds;
    swf::Rectangle<swf::Twips> edge_bounds;
    std::vector<DrawingPath> paths;
    std::vector<render::BitmapInfo> bitmaps;
    std::optional<DrawingFill> current_fill;
    std::optional<DrawingLine> current_line;
    std::vector<DrawingLine> pending_lines;
    swf::Point<swf::Twips> cursor;
    swf::Point<swf::Twips> fill_start;
    FillRule default_winding_rule;
    bool is_empty;

public:
    /// Create a new empty drawing
    Drawing()
        : shape_bounds()
        , edge_bounds()
        , cursor{0, 0}
        , fill_start{0, 0}
        , default_winding_rule(FillRule::EvenOdd)
        , is_empty(true) {}
    
    /// Create from SWF shape
    static Drawing from_swf_shape(const swf::Shape& shape) {
        Drawing drawing;
        drawing.shape_bounds = shape.shape_bounds;
        drawing.edge_bounds = shape.edge_bounds;
        drawing.default_winding_rule = 
            (shape.flags & swf::ShapeFlag::NonZeroWindingRule) 
                ? FillRule::NonZero 
                : FillRule::EvenOdd;
        
        // Convert shape paths to drawing paths
        // TODO: Implement path conversion from DistilledShape
        
        return drawing;
    }
    
    /// Mark the drawing as dirty (needs re-rendering)
    void mark_dirty() {
        is_empty = false;
        render_handle.reset();
    }
    
    /// Set fill style and reset fill rule to default
    void set_fill_style(std::optional<swf::FillStyle> style) {
        new_fill(style, default_winding_rule);
    }
    
    /// Set fill rule and keep the same fill style
    void set_fill_rule(std::optional<FillRule> rule) {
        auto style = current_fill.has_value() 
            ? std::optional(current_fill->style) 
            : std::nullopt;
        new_fill(style, rule.value_or(default_winding_rule));
    }
    
    /// Set fill style and rule
    void new_fill(std::optional<swf::FillStyle> style, std::optional<FillRule> rule) {
        close_path();
        
        if (current_fill.has_value()) {
            paths.push_back(DrawingPath::fill(current_fill.value()));
            current_fill.reset();
        }
        
        // Move pending lines to paths
        for (auto& line : pending_lines) {
            paths.push_back(DrawingPath::line(std::move(line)));
        }
        pending_lines.clear();
        
        if (current_line.has_value()) {
            auto line = current_line.value();
            line.is_closed = (cursor == fill_start);
            auto line_style = line.style;
            paths.push_back(DrawingPath::line(std::move(line)));
            
            current_line = DrawingLine{
                line_style,
                {DrawCommand::move_to(cursor)},
                false
            };
        }
        
        if (style.has_value()) {
            current_fill = DrawingFill{
                style.value(),
                rule.value_or(default_winding_rule),
                {DrawCommand::move_to(cursor)}
            };
        }
        
        fill_start = cursor;
        mark_dirty();
    }
    
    /// Clear the drawing
    void clear() {
        current_fill.reset();
        current_line.reset();
        pending_lines.clear();
        paths.clear();
        bitmaps.clear();
        edge_bounds = swf::Rectangle<swf::Twips>();
        shape_bounds = swf::Rectangle<swf::Twips>();
        cursor = {0, 0};
        fill_start = {0, 0};
        is_empty = true;
        render_handle.reset();
    }
    
    /// Set line style
    void set_line_style(std::optional<swf::LineStyle> style) {
        if (current_line.has_value()) {
            auto line = current_line.value();
            line.is_closed = (cursor == fill_start);
            if (current_fill.has_value()) {
                pending_lines.push_back(std::move(line));
            } else {
                paths.push_back(DrawingPath::line(std::move(line)));
            }
            current_line.reset();
        }
        
        if (style.has_value()) {
            current_line = DrawingLine{
                style.value(),
                {DrawCommand::move_to(cursor)},
                false
            };
        }
        
        mark_dirty();
    }
    
    /// Set line fill style
    void set_line_fill_style(const swf::FillStyle& fill_style) {
        if (current_line.has_value()) {
            auto style = current_line->style;
            // TODO: style.with_fill_style(fill_style)
            set_line_style(std::optional(style));
        }
    }
    
    /// Draw a command
    void draw_command(const DrawCommand& command) {
        bool add_to_bounds = true;
        
        if (auto move_to = command.as_move_to()) {
            close_path();
            fill_start = *move_to;
            add_to_bounds = false;
        }
        
        // Add command to current fill
        if (current_fill.has_value()) {
            current_fill->commands.push_back(command);
        }
        
        // Add command to current line
        auto stroke_width = swf::Twips::zero();
        if (current_line.has_value()) {
            current_line->commands.push_back(command);
            stroke_width = current_line->style.width();
        }
        
        // Expand bounds
        if (add_to_bounds) {
            if (fill_start == cursor) {
                shape_bounds = stretch_bounds(
                    shape_bounds, 
                    DrawCommand::move_to(cursor), 
                    stroke_width, 
                    cursor
                );
                edge_bounds = stretch_bounds(
                    edge_bounds, 
                    DrawCommand::move_to(cursor), 
                    swf::Twips::zero(), 
                    cursor
                );
            }
            shape_bounds = stretch_bounds(shape_bounds, command, stroke_width, cursor);
            edge_bounds = stretch_bounds(edge_bounds, command, swf::Twips::zero(), cursor);
        }
        
        // Update cursor
        if (auto end = command.end_point()) {
            cursor = *end;
        }
        
        mark_dirty();
    }
    
    /// Add a bitmap to the drawing
    uint16_t add_bitmap(const render::BitmapInfo& bitmap) {
        uint16_t id = static_cast<uint16_t>(bitmaps.size());
        bitmaps.push_back(bitmap);
        return id;
    }
    
    /// Obtain a ShapeHandle that represents this Drawing
    std::optional<ShapeHandle> register_or_replace(render::RenderBackend* renderer) {
        if (is_empty) {
            return std::nullopt;
        }
        
        if (!render_handle.has_value()) {
            // Create shape from paths
            // TODO: Implement shape registration
        }
        
        return render_handle;
    }
    
    /// Render the drawing
    void render(RenderContext<GC>* context) {
        if (auto handle = register_or_replace(context->renderer)) {
            context->commands.render_shape(*handle, context->transform_stack.transform());
        }
    }
    
    /// Get self bounds
    swf::Rectangle<swf::Twips> self_bounds() const {
        return shape_bounds;
    }
    
    /// Hit test against the drawing
    bool hit_test(
        const swf::Point<swf::Twips>& point,
        const render::Matrix& local_matrix
    ) const {
        // TODO: Implement hit testing using shape_utils
        return false;
    }
    
    /// Close the current path
    void close_path() {
        if (current_fill.has_value() && cursor != fill_start) {
            current_fill->commands.push_back(DrawCommand::line_to(fill_start));
            if (current_line.has_value()) {
                current_line->commands.push_back(DrawCommand::line_to(fill_start));
            }
            mark_dirty();
        }
    }
    
    /// Check if drawing is empty
    bool is_empty_drawing() const {
        return is_empty;
    }

private:
    static swf::Rectangle<swf::Twips> stretch_bounds(
        const swf::Rectangle<swf::Twips>& bounds,
        const DrawCommand& command,
        swf::Twips stroke_width,
        const swf::Point<swf::Twips>& from
    ) {
        // TODO: Implement bounds stretching based on command type
        return bounds;
    }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_DRAWING_H
