// C++ translation of render/src/lines.rs
// Line drawing utilities for rendering backends

#ifndef RUFFLE_RENDER_LINES_H
#define RUFFLE_RENDER_LINES_H

// Forward declarations
namespace ruffle {
namespace render {
    struct Matrix;
    class CommandHandler;
}
namespace swf {
    struct Color;
    template<typename T>
    struct Point;
    template<typename T>
    struct PointDelta;
    class Twips;
}
}

namespace ruffle {
namespace render {

/// Draw a line using CommandHandler::draw_rect().
///
/// This is a universal way of drawing a line, independent of
/// the rendering backend's implementation and rasterization rules.
///
/// The line is drawn from the origin (0,0) transformed by the matrix
/// to (1,0) transformed by the matrix, creating a 1-pixel wide line.
///
/// \param handler The command handler to draw with
/// \param color The color of the line
/// \param matrix The transformation matrix defining the line position and orientation
void emulate_line(CommandHandler* handler, const swf::Color& color, const Matrix& matrix);

/// Similar to emulate_line(), but emulates drawing a rectangle with lines as its sides.
///
/// This draws four lines forming a rectangle:
/// - Top edge from matrix*(0,0) to matrix*(1,0)
/// - Right edge from matrix*(1,0) to matrix*(1,1)
/// - Bottom edge from matrix*(1,1) to matrix*(0,1)
/// - Left edge from matrix*(0,1) to matrix*(0,0)
///
/// \param handler The command handler to draw with
/// \param color The color of the lines
/// \param matrix The transformation matrix defining the rectangle
void emulate_line_rect(CommandHandler* handler, const swf::Color& color, const Matrix& matrix);

/// Internal helper: Draw a line as a 1px wide rectangle between two points.
///
/// Note that we cannot simply create a rect and then transform it using
/// the transformation matrix, as the thickness and line caps should not be transformed.
/// That's why two already transformed Points are taken as input,
/// and then a 1px wide rectangle is fit inbetween them.
///
/// The algorithm:
/// 1. Calculate the length and angle of the line
/// 2. Create a 1px thick line with the proper length
/// 3. Rotate it so it points from a to b
/// 4. Translate it to start at point a
///
/// \param handler The command handler to draw with
/// \param color The color of the line
/// \param a The start point (already transformed)
/// \param b The end point (already transformed)
void emulate_line_as_rect(
    CommandHandler* handler,
    const swf::Color& color,
    const swf::Point<swf::Twips>& a,
    const swf::Point<swf::Twips>& b
);

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_LINES_H
