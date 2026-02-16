// C++ translation of render/src/blend.rs
// Extended blend mode enumeration

#ifndef RUFFLE_RENDER_BLEND_H
#define RUFFLE_RENDER_BLEND_H

#include <string>
#include <optional>

namespace ruffle {
namespace render {

/// Like `swf::BlendMode`, but contains variants that cannot be read from
/// a SWF (currently, just `Shader`).
enum class ExtendedBlendMode {
    Normal,
    Layer,
    Multiply,
    Screen,
    Lighten,
    Darken,
    Difference,
    Add,
    Subtract,
    Invert,
    Alpha,
    Erase,
    Overlay,
    HardLight,
    Shader,
};

/// Parse ExtendedBlendMode from string
/// Returns std::nullopt if the string is not a valid blend mode name
inline std::optional<ExtendedBlendMode> extended_blend_mode_from_str(const std::string& s) {
    if (s == "normal") return ExtendedBlendMode::Normal;
    if (s == "layer") return ExtendedBlendMode::Layer;
    if (s == "multiply") return ExtendedBlendMode::Multiply;
    if (s == "screen") return ExtendedBlendMode::Screen;
    if (s == "lighten") return ExtendedBlendMode::Lighten;
    if (s == "darken") return ExtendedBlendMode::Darken;
    if (s == "difference") return ExtendedBlendMode::Difference;
    if (s == "add") return ExtendedBlendMode::Add;
    if (s == "subtract") return ExtendedBlendMode::Subtract;
    if (s == "invert") return ExtendedBlendMode::Invert;
    if (s == "alpha") return ExtendedBlendMode::Alpha;
    if (s == "erase") return ExtendedBlendMode::Erase;
    if (s == "overlay") return ExtendedBlendMode::Overlay;
    if (s == "hardlight") return ExtendedBlendMode::HardLight;
    if (s == "shader") return ExtendedBlendMode::Shader;
    return std::nullopt;
}

/// Convert ExtendedBlendMode to string
inline const char* extended_blend_mode_to_str(ExtendedBlendMode mode) {
    switch (mode) {
        case ExtendedBlendMode::Normal: return "normal";
        case ExtendedBlendMode::Layer: return "layer";
        case ExtendedBlendMode::Multiply: return "multiply";
        case ExtendedBlendMode::Screen: return "screen";
        case ExtendedBlendMode::Lighten: return "lighten";
        case ExtendedBlendMode::Darken: return "darken";
        case ExtendedBlendMode::Difference: return "difference";
        case ExtendedBlendMode::Add: return "add";
        case ExtendedBlendMode::Subtract: return "subtract";
        case ExtendedBlendMode::Invert: return "invert";
        case ExtendedBlendMode::Alpha: return "alpha";
        case ExtendedBlendMode::Erase: return "erase";
        case ExtendedBlendMode::Overlay: return "overlay";
        case ExtendedBlendMode::HardLight: return "hardlight";
        case ExtendedBlendMode::Shader: return "shader";
    }
    return "normal"; // Default fallback
}

} // namespace render
} // namespace ruffle

#endif // RUFFLE_RENDER_BLEND_H
