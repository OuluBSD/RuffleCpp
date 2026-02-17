// C++ translation of core/src/character.rs
// Character types and bitmap handling

#ifndef RUFFLE_CORE_CHARACTER_H
#define RUFFLE_CORE_CHARACTER_H

#include <memory>
#include <vector>
#include <cstdint>
#include <optional>
#include <variant>

// Forward declarations
namespace ruffle {
namespace render {
namespace bitmap {
    struct BitmapSize;
    class Bitmap;
}
namespace error {
    class Error;
}
namespace backend {
    class RenderBackend;
}
}

namespace swf {
    struct DefineBitsLossless;
}

namespace common {
class BinaryData;
}

namespace core {
namespace backend {
namespace audio {
    using SoundHandle = uint64_t;  // Placeholder for actual SoundHandle type
}
}

namespace display_object {
    class EditText;
    class Graphic;
    class MovieClip;
    class Avm1Button;
    class Avm2Button;
    class MorphShape;
    class Text;
    class Video;

    enum class BitmapClass;
}

namespace font {
    class Font;
}

// Forward declaration for GC types
template<typename GC> class Gc;
template<typename GC> class GcWeak;
template<typename GC> class Mutation;
template<typename GC> class Lock;

} // namespace core
} // namespace ruffle

namespace ruffle {
namespace core {

/// Bitmap character data
template<typename GC>
class BitmapCharacter {
private:
    CompressedBitmap compressed_;
    std::optional<BitmapHandle> handle_;  // Lazy GPU handle
    Lock<GC, BitmapClass> avm2_class_;

public:
    explicit BitmapCharacter(CompressedBitmap compressed);

    const CompressedBitmap& compressed() const { return compressed_; }
    BitmapClass avm2_class() const;

    static void set_avm2_class(
        Gc<GC, BitmapCharacter> this_ptr,
        BitmapClass bitmap_class,
        Mutation<GC>* mc
    );

    Result<BitmapHandle, render::error::Error> bitmap_handle(
        render::backend::RenderBackend* backend
    );
};

/// Holds a bitmap from an SWF tag, plus the decoded width/height.
/// We avoid decompressing the image until it's actually needed - some pathological SWFS
/// like 'House' have thousands of highly-compressed (mostly empty) bitmaps, which can
/// take over 10GB of ram if we decompress them all during preloading.
class CompressedBitmap {
public:
    /// JPEG bitmap with optional alpha channel
    struct Jpeg {
        std::vector<uint8_t> data;
        std::optional<std::vector<uint8_t>> alpha;
        uint32_t width;
        uint32_t height;
    };

    /// Lossless bitmap
    struct Lossless {
        swf::DefineBitsLossless define_bits_lossless;
    };

    using Data = std::variant<Jpeg, Lossless>;

private:
    Data data_;

public:
    explicit CompressedBitmap(Jpeg jpeg)
        : data_(std::move(jpeg)) {}

    explicit CompressedBitmap(Lossless lossless)
        : data_(std::move(lossless)) {}

    /// Get the bitmap size
    render::bitmap::BitmapSize size() const;

    /// Decode the bitmap
    Result<render::bitmap::Bitmap, render::error::Error> decode() const;
};

/// Character enum - represents different types of characters in the SWF
template<typename GC>
class Character {
public:
    using Data = std::variant<
        display_object::EditText<GC>,
        display_object::Graphic<GC>,
        display_object::MovieClip<GC>,
        Gc<GC, BitmapCharacter<GC>>,
        display_object::Avm1Button<GC>,
        display_object::Avm2Button<GC>,
        font::Font<GC>,
        display_object::MorphShape<GC>,
        display_object::Text<GC>,
        backend::audio::SoundHandle,
        display_object::Video<GC>,
        std::shared_ptr<common::BinaryData>
    >;

private:
    Data data_;

public:
    Character() = default;

    // Variant constructors
    explicit Character(display_object::EditText<GC> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::Graphic<GC> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::MovieClip<GC> obj) : data_(std::move(obj)) {}
    explicit Character(Gc<GC, BitmapCharacter<GC>> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::Avm1Button<GC> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::Avm2Button<GC> obj) : data_(std::move(obj)) {}
    explicit Character(font::Font<GC> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::MorphShape<GC> obj) : data_(std::move(obj)) {}
    explicit Character(display_object::Text<GC> obj) : data_(std::move(obj)) {}
    explicit Character(backend::audio::SoundHandle handle) : data_(handle) {}
    explicit Character(display_object::Video<GC> obj) : data_(std::move(obj)) {}
    explicit Character(std::shared_ptr<common::BinaryData> data) : data_(std::move(data)) {}

    const Data& data() const { return data_; }

    // Type checks
    bool is_edit_text() const { return std::holds_alternative<display_object::EditText<GC>>(data_); }
    bool is_graphic() const { return std::holds_alternative<display_object::Graphic<GC>>(data_); }
    bool is_movie_clip() const { return std::holds_alternative<display_object::MovieClip<GC>>(data_); }
    bool is_bitmap() const { return std::holds_alternative<Gc<GC, BitmapCharacter<GC>>>(data_); }
    bool is_avm1_button() const { return std::holds_alternative<display_object::Avm1Button<GC>>(data_); }
    bool is_avm2_button() const { return std::holds_alternative<display_object::Avm2Button<GC>>(data_); }
    bool is_font() const { return std::holds_alternative<font::Font<GC>>(data_); }
    bool is_morph_shape() const { return std::holds_alternative<display_object::MorphShape<GC>>(data_); }
    bool is_text() const { return std::holds_alternative<display_object::Text<GC>>(data_); }
    bool is_sound() const { return std::holds_alternative<backend::audio::SoundHandle>(data_); }
    bool is_video() const { return std::holds_alternative<display_object::Video<GC>>(data_); }
    bool is_binary_data() const { return std::holds_alternative<std::shared_ptr<common::BinaryData>>(data_); }
};

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_CHARACTER_H
