// C++ translation of web/src/ui/font_renderer.rs
// Canvas-based font renderer for web platform

#ifndef RUFFLE_WEB_UI_FONT_RENDERER_H
#define RUFFLE_WEB_UI_FONT_RENDERER_H

#include <string>
#include <optional>
#include <memory>

// Forward declarations
namespace ruffle {
namespace core {
namespace font {
    struct FontMetrics;
    struct Glyph;
    class FontRenderer;
}
namespace swf {
    struct Twips;
}
}
namespace render {
namespace bitmap {
    class Bitmap;
    enum class BitmapFormat;
}
}
}

// Web/JS types (would be provided by wasm-bindgen)
namespace web_sys {
    class OffscreenCanvas;
    class OffscreenCanvasRenderingContext2d;
}

namespace js_sys {
    class JsValue;
}

namespace ruffle {
namespace web {
namespace ui {
namespace font_renderer {

/// Canvas-based font renderer for web platform
///
/// Uses OffscreenCanvas to render font glyphs and measure text metrics.
/// Renders fonts at 64px size and scales metrics to twips precision.
class CanvasFontRenderer : public core::font::FontRenderer {
private:
    /// Offscreen canvas for rendering
    std::shared_ptr<web_sys::OffscreenCanvas> canvas_;

    /// 2D rendering context
    std::shared_ptr<web_sys::OffscreenCanvasRenderingContext2d> ctx_;

    /// Font string (CSS format)
    std::string font_str_;

    /// Font ascent in pixels
    double ascent_;

    /// Font descent in pixels
    double descent_;

    /// Render fonts with size 64px. It affects the bitmap size.
    static constexpr double SIZE_PX = 64.0;

    /// Divide each pixel into 20 (use twips precision). It affects metrics.
    static constexpr double SCALE = 20.0;

    /// Check if OffscreenCanvas is supported in the current environment
    ///
    /// \return true if OffscreenCanvas is supported, false otherwise
    static bool is_offscreen_canvas_supported();

    /// Create a CSS font string from font properties
    ///
    /// \param italic Whether the font is italic
    /// \param bold Whether the font is bold
    /// \param size Font size in pixels
    /// \param font_family Font family name
    /// \return CSS font string
    static std::string to_font_str(bool italic, bool bold, double size, const std::string& font_family);

    /// Apply font style to the canvas context
    ///
    /// \param ctx The rendering context
    /// \param font_str The font string to apply
    static void apply_style(const web_sys::OffscreenCanvasRenderingContext2d& ctx, const std::string& font_str);

    /// Calculate the width of text in pixels
    ///
    /// \param text The text to measure
    /// \return Width in pixels
    /// \throws JsValue on measurement failure
    double calculate_width(const std::string& text) const;

    /// Ensure the canvas is large enough for the given dimensions
    ///
    /// \param width Required width in pixels
    /// \param height Required height in pixels
    void ensure_canvas_large_enough(double width, double height);

    /// Render a single glyph to a bitmap
    ///
    /// \param character The character to render
    /// \return The rendered glyph
    /// \throws JsValue on rendering failure
    core::font::Glyph render_glyph_internal(char32_t character) const;

    /// Calculate kerning between two characters
    ///
    /// \param left The left character
    /// \param right The right character
    /// \return Kerning offset in twips
    /// \throws JsValue on calculation failure
    core::swf::Twips calculate_kerning_internal(char32_t left, char32_t right) const;

public:
    /// Create a new canvas font renderer
    ///
    /// \param italic Whether the font is italic
    /// \param bold Whether the font is bold
    /// \param font_family The font family name
    /// \return New CanvasFontRenderer instance
    /// \throws JsValue on creation failure
    static std::shared_ptr<CanvasFontRenderer> new_renderer(
        bool italic, bool bold, const std::string& font_family);

    /// Get the font metrics
    ///
    /// \return Font metrics including scale, ascent, descent, and leading
    core::font::FontMetrics get_font_metrics() const override;

    /// Check if this renderer has kerning information
    ///
    /// \return true (canvas renderer always has kerning info)
    bool has_kerning_info() const override;

    /// Render a glyph for the given character
    ///
    /// \param character The Unicode character to render
    /// \return The rendered glyph, or nullopt on failure
    std::optional<core::font::Glyph> render_glyph(char32_t character) const override;

    /// Calculate kerning between two characters
    ///
    /// \param left The left character
    /// \param right The right character
    /// \return Kerning offset in twips (ZERO on failure)
    core::swf::Twips calculate_kerning(char32_t left, char32_t right) const override;
};

} // namespace font_renderer
} // namespace ui
} // namespace web
} // namespace ruffle

#endif // RUFFLE_WEB_UI_FONT_RENDERER_H
